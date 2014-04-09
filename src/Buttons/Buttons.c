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
	uint8_t USARTBuffer[USART_DATA_LENGTH_BUTTON];
	uint8_t buttonUSART = 0;

#if (BUTTONS_MULTIPLEX == BUTTONS_MULTIPLEX_HC165)
	HC165__readByte(&buttonWired);
#else
	buttonWired = (		(isLow(BUTTON_MODE_PORT, BUTTON_MODE_PIN) << BITPOS_BUTTON_MODE)
					| 	(isLow(BUTTON_FUNC1_PORT, BUTTON_FUNC1_PIN) << BITPOS_BUTTON_FUNC1)
					| 	(isLow(BUTTON_FUNC2_PORT, BUTTON_FUNC2_PIN) << BITPOS_BUTTON_FUNC2)
					);
#endif

	if (E_OK == (USART__readDataBytes(USARTBuffer, USART_DATA_LENGTH_BUTTON, USART_REQESTER_BUTTON)))
	{
		buttonUSART = USARTBuffer[1];
	}

	/* update buttons states */
	Buttons__updateState(buttonWired | buttonUSART, &buttonMode);
	Buttons__updateState(buttonWired | buttonUSART, &buttonFunc1);
	Buttons__updateState(buttonWired | buttonUSART, &buttonFunc2);
	Buttons__updateState(buttonWired | buttonUSART, &buttonFunc3);
	Buttons__updateState(buttonWired | buttonUSART, &buttonLeft);
	Buttons__updateState(buttonWired | buttonUSART, &buttonRight);
	Buttons__updateState(buttonWired | buttonUSART, &buttonUp);
	Buttons__updateState(buttonWired | buttonUSART, &buttonDown);

	Buttons__updateState_USART(buttonUSART, &buttonMode);
	Buttons__updateState_USART(buttonUSART, &buttonFunc1);
	Buttons__updateState_USART(buttonUSART, &buttonFunc2);
	Buttons__updateState_USART(buttonUSART, &buttonFunc3);
	Buttons__updateState_USART(buttonUSART, &buttonLeft);
	Buttons__updateState_USART(buttonUSART, &buttonRight);
	Buttons__updateState_USART(buttonUSART, &buttonUp);
	Buttons__updateState_USART(buttonUSART, &buttonDown);

	if (buttonWired != 0)
	{
		setHigh(TEST_LED_PORT, TEST_LED_PIN);
	}
	else
	{
		setLow(TEST_LED_PORT, TEST_LED_PIN);
	}
}


uint8_t Buttons__isPressed (Button_t *button)
{
	return button->pressed;
}


uint8_t Buttons__isPressedOnce (Button_t *button)
{
	return (button->pressedOnce || button->pressedUSART);
}
