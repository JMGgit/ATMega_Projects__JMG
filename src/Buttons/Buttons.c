/*
 * Buttons.c
 *
 *  Created on: 02.03.2013
 *      Author: Jean-Martin George
 */


#include "Buttons.h"


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
#if (BUTTONS_MULTIPLEX == BUTTONS_MULTIPLEX_OFF)
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


static void Buttons__updateState (uint8_t receiveBuffer, Button_t *button)
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


static void Buttons__updateState_USART (uint8_t receiveBuffer, Button_t *button)
{
	if (receiveBuffer & (button->bitMask))
	{
		button->pressedUSART = TRUE;
	}
	else
	{
		button->pressedUSART = FALSE;
	}
}


void Buttons__x10 (void)
{
	uint8_t buttonWired = 0;
#if (BUTTONS_USART == BUTTONS_USART_ON)
	uint8_t USARTBuffer[USART_DATA_LENGTH_BUTTON];
#endif
#if (BUTTONS_IRMP == BUTTONS_IRMP_ON)
	uint8_t IRMPBuffer;
#endif
	uint8_t buttonUSART = 0;
	uint8_t buttonIRMP = 0;

#if (BUTTONS_MULTIPLEX == BUTTONS_MULTIPLEX_HC165)
	HC165__readByte(&buttonWired);
#else
	buttonWired = (		(Button__isLow(BUTTON_MODE_PORT, BUTTON_MODE_PIN) << BITPOS_BUTTON_MODE)
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
	if (E_OK == IRMP__readData(&IRMPBuffer, 1))
	{
		if (IRMPBuffer == IRMP_BUTTON_MODE)
		{
			buttonIRMP = BUTTON_MODE;
		}
		else if (IRMPBuffer == IRMP_BUTTON_FUNC1)
		{
			buttonIRMP = BUTTON_FUNC1;
		}
		else if (IRMPBuffer == IRMP_BUTTON_FUNC2)
		{
			buttonIRMP = BUTTON_FUNC2;
		}
		else if (IRMPBuffer == IRMP_BUTTON_FUNC3)
		{
			buttonIRMP = BUTTON_FUNC3;
		}
		else if (IRMPBuffer == IRMP_BUTTON_UP)
		{
			buttonIRMP = BUTTON_UP;
		}
		else if (IRMPBuffer == IRMP_BUTTON_DOWN)
		{
			buttonIRMP = BUTTON_DOWN;
		}
		else if (IRMPBuffer == IRMP_BUTTON_LEFT)
		{
			buttonIRMP = BUTTON_LEFT;
		}
		else if (IRMPBuffer == IRMP_BUTTON_RIGHT)
		{
			buttonIRMP = BUTTON_RIGHT;
		}
	}
#endif

	/* update buttons states */
	Buttons__updateState(buttonWired | buttonUSART | buttonIRMP, &buttonMode);
	Buttons__updateState(buttonWired | buttonUSART | buttonIRMP, &buttonFunc1);
	Buttons__updateState(buttonWired | buttonUSART | buttonIRMP, &buttonFunc2);
	Buttons__updateState(buttonWired | buttonUSART | buttonIRMP, &buttonFunc3);
	Buttons__updateState(buttonWired | buttonUSART | buttonIRMP, &buttonLeft);
	Buttons__updateState(buttonWired | buttonUSART | buttonIRMP, &buttonRight);
	Buttons__updateState(buttonWired | buttonUSART | buttonIRMP, &buttonUp);
	Buttons__updateState(buttonWired | buttonUSART | buttonIRMP, &buttonDown);

	Buttons__updateState_USART(buttonUSART, &buttonMode);
	Buttons__updateState_USART(buttonUSART, &buttonFunc1);
	Buttons__updateState_USART(buttonUSART, &buttonFunc2);
	Buttons__updateState_USART(buttonUSART, &buttonFunc3);
	Buttons__updateState_USART(buttonUSART, &buttonLeft);
	Buttons__updateState_USART(buttonUSART, &buttonRight);
	Buttons__updateState_USART(buttonUSART, &buttonUp);
	Buttons__updateState_USART(buttonUSART, &buttonDown);
}


uint8_t Buttons__isPressed (Button_t *button)
{
	return button->pressed;
}


uint8_t Buttons__isPressedOnce (Button_t *button)
{
	return (button->pressedOnce || button->pressedUSART);
}
