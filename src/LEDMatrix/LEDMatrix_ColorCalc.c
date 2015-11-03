/*
 * LEDMatrix_ColorCalc.c
 *
 *  Created on: 03.11.2015
 *      Author: Jean-Martin George
 */


#include "LEDMatrix_ColorCalc.h"

static RGB_Color_t currentColor = {255, 255, 255};


RGB_Color_t LEDMatrix__getColorFromInputs (void)
{
#if (BUTTONS_USART == BUTTONS_USART_ON)
	uint8_t USARTbuffer[USART_DATA_LENGTH_COLOR];

	if (E_OK == USART__readData(USARTbuffer, USART_DATA_LENGTH_COLOR, USART_REQESTER_COLOR))
	{
		currentColor.red = USARTbuffer[1];
		currentColor.green = USARTbuffer[2];
		currentColor.blue = USARTbuffer[3];
	}
#endif

	if (Buttons__isPressed(&buttonFunc1))
	{
		currentColor.red--;
	}

	if (Buttons__isPressed(&buttonFunc2))
	{
		currentColor.green--;
	}

	if (Buttons__isPressed(&buttonFunc3))
	{
		currentColor.blue--;
	}

	return currentColor;
}
