/*
 * Buttons.c
 *
 *  Created on: 02.03.2013
 *      Author: Jean-Martin George
 */


#include "Buttons.h"


Button_t buttonOff;
Button_t buttonMode;
Button_t buttonFunc1;
Button_t buttonFunc2;
Button_t buttonFunc3;
Button_t buttonLeft;
Button_t buttonRight;
Button_t buttonUp;
Button_t buttonDown;

#if (BUTTONS_IRMP == BUTTONS_IRMP_SEND_TO_TWI)
uint16_t buttonTWI;
uint8_t twiTxBuffer[2];
#endif

void Buttons__init (void)
{
    buttonOff.bitMask = BITMSK_BUTTON_OFF;
    buttonMode.bitMask = BITMSK_BUTTON_MODE;
    buttonFunc1.bitMask = BITMSK_BUTTON_FUNC1;
    buttonFunc2.bitMask = BITMSK_BUTTON_FUNC2;
    buttonFunc3.bitMask = BITMSK_BUTTON_FUNC3;
    buttonLeft.bitMask = BITMSK_BUTTON_LEFT;
    buttonRight.bitMask = BITMSK_BUTTON_RIGHT;
    buttonUp.bitMask = BITMSK_BUTTON_UP;
    buttonDown.bitMask = BITMSK_BUTTON_DOWN;

#if (BUTTONS_WIRED == BUTTONS_WIRED_PIN)
	Button__initButton(BUTTON_OFF_DDR, BUTTON_OFF_PORT, BUTTON_OFF_PIN);
	Button__initButton(BUTTON_MODE_DDR, BUTTON_MODE_PORT, BUTTON_MODE_PIN);
	Button__initButton(BUTTON_FUNC1_DDR, BUTTON_FUNC1_PORT, BUTTON_FUNC1_PIN);
	Button__initButton(BUTTON_FUNC2_DDR, BUTTON_FUNC2_PORT, BUTTON_FUNC2_PIN);
	Button__initButton(BUTTON_FUNC3_DDR, BUTTON_FUNC3_PORT, BUTTON_FUNC3_PIN);
	Button__initButton(BUTTON_LEFT_DDR, BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);
	Button__initButton(BUTTON_RIGHT_DDR, BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN);
	Button__initButton(BUTTON_UP_DDR, BUTTON_UP_PORT, BUTTON_UP_PIN);
	Button__initButton(BUTTON_DOWN_DDR, BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
#endif
#if (BUTTONS_IRMP == BUTTONS_IRMP_SEND_TO_TWI)
	TWI__slaveInit (twiTxBuffer, 2);
#endif
}


#if (BUTTONS_WIRED != BUTTONS_WIRED_OFF)
static void Buttons__updateState (uint16_t receiveBuffer, Button_t *button)
{
	if (receiveBuffer & (button->bitMask))
	{
		if (button->debounceTime + uC__getTaskIncrement() < 0xFF)
		{
			button->debounceTime  = button->debounceTime + uC__getTaskIncrement();
		}
	}
	else
	{
		button->debounceTime = 0;
	}

	if (button->debounceTime > BUTTON_DEBOUNCE_TIME)
	{
		button->pressed = TRUE;
		button->pressedOnce = FALSE;
	}
	else if (button->debounceTime == BUTTON_DEBOUNCE_TIME)
	{
		button->pressedOnce = TRUE;
	}
	else
	{
		button->pressed = FALSE;
		button->pressedOnce = FALSE;
	}
}
#endif


#if (BUTTONS_USART == BUTTONS_USART_ON)
static void Buttons__updateState_USART (uint16_t receiveBuffer, Button_t *button)
{
	if (receiveBuffer & (button->bitMask))
	{
		button->pressedUSART = TRUE;
		button->pressedOnceUSART = TRUE;
	}
	else
	{
		button->pressedUSART = FALSE;
		button->pressedOnceUSART = FALSE;
	}
}
#endif


#if (BUTTONS_IRMP != BUTTONS_IRMP_OFF)
static void Buttons__updateState_IRMP (uint8_t signalValid, uint16_t receiveBuffer, uint8_t repeat, Button_t *button)
{
	if (signalValid)
	{
		if (receiveBuffer & (button->bitMask))
		{
			if (!(button->pressedIRMP) && (!repeat))
			{
				button->pressedOnceIRMP = TRUE;
			}
			else
			{
				button->pressedOnceIRMP = FALSE;
			}

			button->pressedIRMP = TRUE;
			button->debounceTimeIRMP = IRMP_DEBOUNCE_TIME;
		}
		else
		{
			button->pressedIRMP = FALSE;
			button->pressedOnceIRMP = FALSE;
		}
	}
	else
	{
		if (button->debounceTimeIRMP - uC__getTaskIncrement() > 0)
		{
			button->debounceTimeIRMP = button->debounceTimeIRMP - uC__getTaskIncrement();
		}
		else
		{
			button->debounceTimeIRMP = IRMP_DEBOUNCE_TIME;
			button->pressedIRMP = FALSE;

		}

		button->pressedOnceIRMP = FALSE;
	}
}
#endif


#if (BUTTONS_TWI == BUTTONS_TWI_ON)
static void Buttons__updateState_TWI (uint16_t receiveBuffer, Button_t *button)
{
	if (receiveBuffer & (button->bitMask))
	{
		if (button->debounceTime + uC__getTaskIncrement() < 0xFF)
		{
			button->debounceTime  = button->debounceTime + uC__getTaskIncrement();
		}
	}
	else
	{
		button->debounceTime = 0;
	}

	if (button->debounceTime > BUTTON_DEBOUNCE_TIME)
	{
		button->pressedTWI = TRUE;
		button->pressedOnceTWI = FALSE;
	}
	else if (button->debounceTime == BUTTON_DEBOUNCE_TIME)
	{
		button->pressedOnceTWI = TRUE;
	}
	else
	{
		button->pressedTWI = FALSE;
		button->pressedOnceTWI = FALSE;
	}
}
#endif


void Buttons__x10 (void)
{
#if (BUTTONS_WIRED != BUTTONS_WIRED_OFF)
	uint16_t buttonWired = 0;

#if (BUTTONS_WIRED == BUTTONS_WIRED_HC165)
	uint8_t buttonMultiplex;

	HC165__readByte(&buttonMultiplex);
	buttonWired = buttonMultiplex;
#elif (BUTTONS_WIRED == BUTTONS_WIRED_PIN)
	buttonWired = (		(Button__isLow(BUTTON_OFF_PORT, BUTTON_OFF_PIN) << BITPOS_BUTTON_OFF)
					|	(Button__isLow(BUTTON_MODE_PORT, BUTTON_MODE_PIN) << BITPOS_BUTTON_MODE)
					| 	(Button__isLow(BUTTON_FUNC1_PORT, BUTTON_FUNC1_PIN) << BITPOS_BUTTON_FUNC1)
					| 	(Button__isLow(BUTTON_FUNC2_PORT, BUTTON_FUNC2_PIN) << BITPOS_BUTTON_FUNC2)
					| 	(Button__isLow(BUTTON_FUNC3_PORT, BUTTON_FUNC3_PIN) << BITPOS_BUTTON_FUNC3)
					| 	(Button__isLow(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN) << BITPOS_BUTTON_LEFT)
					| 	(Button__isLow(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN) << BITPOS_BUTTON_RIGHT)
					| 	(Button__isLow(BUTTON_UP_PORT, BUTTON_UP_PIN) << BITPOS_BUTTON_UP)
					| 	(Button__isLow(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN) << BITPOS_BUTTON_DOWN)
					);
#endif

	Buttons__updateState(buttonWired, &buttonOff);
	Buttons__updateState(buttonWired, &buttonMode);
	Buttons__updateState(buttonWired, &buttonFunc1);
	Buttons__updateState(buttonWired, &buttonFunc2);
	Buttons__updateState(buttonWired, &buttonFunc3);
	Buttons__updateState(buttonWired, &buttonLeft);
	Buttons__updateState(buttonWired, &buttonRight);
	Buttons__updateState(buttonWired, &buttonUp);
	Buttons__updateState(buttonWired, &buttonDown);
#endif

#if (BUTTONS_USART == BUTTONS_USART_ON)
	uint8_t USARTBuffer[USART_DATA_LENGTH_BUTTON];
	uint16_t buttonUSART = 0;

	if (E_OK == USART__readData(USARTBuffer, USART_DATA_LENGTH_BUTTON, USART_REQESTER_BUTTON))
	{
		buttonUSART = USARTBuffer[1];
	}

	Buttons__updateState_USART(buttonUSART, &buttonOff);
	Buttons__updateState_USART(buttonUSART, &buttonMode);
	Buttons__updateState_USART(buttonUSART, &buttonFunc1);
	Buttons__updateState_USART(buttonUSART, &buttonFunc2);
	Buttons__updateState_USART(buttonUSART, &buttonFunc3);
	Buttons__updateState_USART(buttonUSART, &buttonLeft);
	Buttons__updateState_USART(buttonUSART, &buttonRight);
	Buttons__updateState_USART(buttonUSART, &buttonUp);
	Buttons__updateState_USART(buttonUSART, &buttonDown);
#endif

#if (BUTTONS_IRMP != BUTTONS_IRMP_OFF)
	uint8_t signalValidIRMP = FALSE;
	uint8_t IRMPBuffer[2];
	uint8_t IRMPBufferVal;
	uint16_t buttonIRMP = 0;
	uint8_t repeatIRMP = FALSE;

	if (E_OK == IRMP__readData(IRMP_REMOTE_ADDRESS, IRMPBuffer, 2, &repeatIRMP))
	{
		signalValidIRMP = TRUE;
		IRMPBufferVal = (IRMPBuffer[0] << 8) | IRMPBuffer[1] ;

		if (IRMPBufferVal == IRMP_BUTTON_OFF)
		{
			buttonIRMP = BUTTON_OFF;
		}
		else if (IRMPBufferVal == IRMP_BUTTON_MODE)
		{
			buttonIRMP = BUTTON_MODE;
		}
		else if (IRMPBufferVal == IRMP_BUTTON_FUNC1)
		{
			buttonIRMP = BUTTON_FUNC1;
		}
		else if (IRMPBufferVal == IRMP_BUTTON_FUNC2)
		{
			buttonIRMP = BUTTON_FUNC2;
		}
		else if (IRMPBufferVal == IRMP_BUTTON_FUNC3)
		{
			buttonIRMP = BUTTON_FUNC3;
		}
		else if (IRMPBufferVal == IRMP_BUTTON_UP)
		{
			buttonIRMP = BUTTON_UP;
		}
		else if (IRMPBufferVal == IRMP_BUTTON_DOWN)
		{
			buttonIRMP = BUTTON_DOWN;
		}
		else if (IRMPBufferVal == IRMP_BUTTON_LEFT)
		{
			buttonIRMP = BUTTON_LEFT;
		}
		else if (IRMPBufferVal == IRMP_BUTTON_RIGHT)
		{
			buttonIRMP = BUTTON_RIGHT;
		}
	}
	else
	{
		signalValidIRMP = FALSE;
	}

	Buttons__updateState_IRMP(signalValidIRMP, buttonIRMP, repeatIRMP, &buttonOff);
	Buttons__updateState_IRMP(signalValidIRMP, buttonIRMP, repeatIRMP, &buttonMode);
	Buttons__updateState_IRMP(signalValidIRMP, buttonIRMP, repeatIRMP, &buttonFunc1);
	Buttons__updateState_IRMP(signalValidIRMP, buttonIRMP, repeatIRMP, &buttonFunc2);
	Buttons__updateState_IRMP(signalValidIRMP, buttonIRMP, repeatIRMP, &buttonFunc3);
	Buttons__updateState_IRMP(signalValidIRMP, buttonIRMP, repeatIRMP, &buttonLeft);
	Buttons__updateState_IRMP(signalValidIRMP, buttonIRMP, repeatIRMP, &buttonRight);
	Buttons__updateState_IRMP(signalValidIRMP, buttonIRMP, repeatIRMP, &buttonUp);
	Buttons__updateState_IRMP(signalValidIRMP, buttonIRMP, repeatIRMP, &buttonDown);

#if (BUTTONS_IRMP == BUTTONS_IRMP_SEND_TO_TWI)
	buttonTWI =
				((buttonOff.pressedIRMP) << BITPOS_BUTTON_OFF)
			|	((buttonMode.pressedIRMP) << BITPOS_BUTTON_MODE)
			|	((buttonFunc1.pressedIRMP) << BITPOS_BUTTON_FUNC1)
			|	((buttonFunc2.pressedIRMP) << BITPOS_BUTTON_FUNC2)
			|	((buttonFunc3.pressedIRMP) << BITPOS_BUTTON_FUNC3)
			|	((buttonLeft.pressedIRMP) << BITPOS_BUTTON_LEFT)
			|	((buttonRight.pressedIRMP) << BITPOS_BUTTON_RIGHT)
			|	((buttonUp.pressedIRMP) << BITPOS_BUTTON_UP)
			|	((buttonDown.pressedIRMP) << BITPOS_BUTTON_DOWN)
			;

	memcpy (twiTxBuffer, &buttonTWI, 2);
	TWI__slaveUpdateTxData();
#endif
#endif

#if (BUTTONS_TWI == BUTTONS_TWI_ON)
	uint16_t buttonTWI = 0;

	uint8_t data[2];
	uint8_t receiveState = E_NOT_OK;

	Debug__setWhileState(WHILE_STATE_BUTTONS_BEFORE);
	while (receiveState != E_OK)
	{
		receiveState = TWI__masterReadData(&data[0], 2, BUTTONS_TWI_SLAVE_ADDRESS);
	}
	Debug__setWhileState(WHILE_STATE_BUTTONS_AFTER);

	receiveState = E_NOT_OK;

	buttonTWI = data[0] + (data[1] << 8);

	Buttons__updateState_TWI(buttonTWI, &buttonOff);
	Buttons__updateState_TWI(buttonTWI, &buttonMode);
	Buttons__updateState_TWI(buttonTWI, &buttonFunc1);
	Buttons__updateState_TWI(buttonTWI, &buttonFunc2);
	Buttons__updateState_TWI(buttonTWI, &buttonFunc3);
	Buttons__updateState_TWI(buttonTWI, &buttonLeft);
	Buttons__updateState_TWI(buttonTWI, &buttonRight);
	Buttons__updateState_TWI(buttonTWI, &buttonUp);
	Buttons__updateState_TWI(buttonTWI, &buttonDown);

	if (		((buttonTWI & BITMSK_BUTTON_OFF) != 0)
			||	((buttonTWI & BITMSK_BUTTON_MODE) != 0)
			||	((buttonTWI & BITMSK_BUTTON_FUNC1) != 0)
			||	((buttonTWI & BITMSK_BUTTON_FUNC2) != 0)
			||	((buttonTWI & BITMSK_BUTTON_FUNC3) != 0)
			||	((buttonTWI & BITMSK_BUTTON_LEFT) != 0)
			||	((buttonTWI & BITMSK_BUTTON_RIGHT) != 0)
			||	((buttonTWI & BITMSK_BUTTON_UP) != 0)
			||	((buttonTWI & BITMSK_BUTTON_DOWN) != 0)
	)
	{
		setHigh(IRMP_LED_PORT, IRMP_LED_PIN);
	}
	else
	{
		setLow(IRMP_LED_PORT, IRMP_LED_PIN);
	}
#endif
}


uint8_t Buttons__isPressed (Button_t *button)
{
	return (button->pressed
#if (BUTTONS_USART == BUTTONS_USART_ON)
			|| button->pressedUSART
#endif
#if (BUTTONS_IRMP != BUTTONS_IRMP_OFF)
			|| button->pressedIRMP
#endif
#if (BUTTONS_TWI == BUTTONS_TWI_ON)
			|| button->pressedTWI
#endif
			);
}


uint8_t Buttons__isPressedOnce (Button_t *button)
{
	return (button->pressedOnce
#if (BUTTONS_USART == BUTTONS_USART_ON)
			|| button->pressedOnceUSART
#endif
#if (BUTTONS_IRMP != BUTTONS_IRMP_OFF)
			|| button->pressedOnceIRMP
#endif
#if (BUTTONS_TWI == BUTTONS_TWI_ON)
			|| button->pressedOnceTWI
#endif
			);
}
