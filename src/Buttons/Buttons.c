/*
 * Buttons.c
 *
 *  Created on: 02.03.2013
 *      Author: Jean-Martin George
 */


#include "Buttons.h"


Button_t buttonOff	 = {BITMSK_BUTTON_OFF,		0, FALSE, FALSE};
Button_t buttonMode  = {BITMSK_BUTTON_MODE,		0, FALSE, FALSE};
Button_t buttonFunc1 = {BITMSK_BUTTON_FUNC1,	0, FALSE, FALSE};
Button_t buttonFunc2 = {BITMSK_BUTTON_FUNC2,	0, FALSE, FALSE};
Button_t buttonFunc3 = {BITMSK_BUTTON_FUNC3,	0, FALSE, FALSE};
Button_t buttonLeft  = {BITMSK_BUTTON_LEFT,		0, FALSE, FALSE};
Button_t buttonRight = {BITMSK_BUTTON_RIGHT,	0, FALSE, FALSE};
Button_t buttonUp    = {BITMSK_BUTTON_UP,		0, FALSE, FALSE};
Button_t buttonDown  = {BITMSK_BUTTON_DOWN,		0, FALSE, FALSE};


void Buttons__init (void)
{
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
}


static void Buttons__updateState (uint16_t receiveBuffer, Button_t *button)
{
	if (receiveBuffer & (button->bitMask))
	{
		if (button->debounceTime < 0xFF)
		{
			button->debounceTime++;
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


#if (BUTTONS_IRMP == BUTTONS_IRMP_ON)
static void Buttons__updateState_IRMP (uint16_t receiveBuffer, uint8_t repeat, Button_t *button)
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
	}
	else
	{
		button->pressedIRMP = FALSE;
		button->pressedOnceIRMP = FALSE;
	}
}
#endif


void Buttons__x10 (void)
{
	uint16_t buttonWired = 0;
#if (BUTTONS_USART == BUTTONS_USART_ON)
	uint8_t USARTBuffer[USART_DATA_LENGTH_BUTTON];
	uint16_t buttonUSART = 0;
#endif

#if (BUTTONS_IRMP == BUTTONS_IRMP_ON)
	uint8_t IRMPBuffer[2];
	uint8_t IRMPBufferVal;
	uint16_t buttonIRMP = 0;
	uint8_t repeatIRMP = FALSE;
#endif

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

#if (BUTTONS_USART == BUTTONS_USART_ON)
	if (E_OK == USART__readData(USARTBuffer, USART_DATA_LENGTH_BUTTON, USART_REQESTER_BUTTON))
	{
		buttonUSART = USARTBuffer[1];
	}
#endif

#if (BUTTONS_IRMP == BUTTONS_IRMP_ON)
	if (E_OK == IRMP__readData(IRMP_REMOTE_ADDRESS, IRMPBuffer, 2, &repeatIRMP))
	{
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
#endif

	/* update buttons states */
	Buttons__updateState(buttonWired, &buttonOff);
	Buttons__updateState(buttonWired, &buttonMode);
	Buttons__updateState(buttonWired, &buttonFunc1);
	Buttons__updateState(buttonWired, &buttonFunc2);
	Buttons__updateState(buttonWired, &buttonFunc3);
	Buttons__updateState(buttonWired, &buttonLeft);
	Buttons__updateState(buttonWired, &buttonRight);
	Buttons__updateState(buttonWired, &buttonUp);
	Buttons__updateState(buttonWired, &buttonDown);

#if (BUTTONS_USART == BUTTONS_USART_ON)
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

#if (BUTTONS_IRMP == BUTTONS_IRMP_ON)
	Buttons__updateState_IRMP(buttonIRMP, repeatIRMP, &buttonOff);
	Buttons__updateState_IRMP(buttonIRMP, repeatIRMP, &buttonMode);
	Buttons__updateState_IRMP(buttonIRMP, repeatIRMP, &buttonFunc1);
	Buttons__updateState_IRMP(buttonIRMP, repeatIRMP, &buttonFunc2);
	Buttons__updateState_IRMP(buttonIRMP, repeatIRMP, &buttonFunc3);
	Buttons__updateState_IRMP(buttonIRMP, repeatIRMP, &buttonLeft);
	Buttons__updateState_IRMP(buttonIRMP, repeatIRMP, &buttonRight);
	Buttons__updateState_IRMP(buttonIRMP, repeatIRMP, &buttonUp);
	Buttons__updateState_IRMP(buttonIRMP, repeatIRMP, &buttonDown);
#endif
}


uint8_t Buttons__isPressed (Button_t *button)
{
	return (button->pressed
#if (BUTTONS_USART == BUTTONS_USART_ON)
			|| button->pressedUSART
#endif
#if (BUTTONS_IRMP == BUTTONS_IRMP_ON)
			|| button->pressedIRMP
#endif
			);
}


uint8_t Buttons__isPressedOnce (Button_t *button)
{
	return (button->pressedOnce
#if (BUTTONS_USART == BUTTONS_USART_ON)
			|| button->pressedOnceUSART
#endif
#if (BUTTONS_IRMP == BUTTONS_IRMP_ON)
			|| button->pressedOnceIRMP
#endif
			);
}
