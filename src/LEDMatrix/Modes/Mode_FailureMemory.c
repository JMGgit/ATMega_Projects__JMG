/*
 * Mode_FailureMemory.c
 *
 *  Created on: 14.10.2016
 *      Author: Jean-Martin George
 */


#include "Mode_FailureMemory.h"


void FailureMemory__x10 (void)
{
#if (DEBUG_MODE == DEBUG_MODE_ON)
	static uint8_t failureTestTimer = 255;
	uint8_t failureCounter = FailureMemory__getFaultCounter();
	uint8_t linIt, colIt, lineNb, colNb;
	RGB_Color_t color;

	LEDMatrix__clearMatrix();

	/* illuminate 4 edges */
	LEDMatrix__setRGBColorFromComponents(&color, 50, 50, 50);

	LEDMatrix__setRGBColor(1, 1, color);
	LEDMatrix__setRGBColor(1, LED_MATRIX_SIZE_COL, color);
	LEDMatrix__setRGBColor(LED_MATRIX_SIZE_LIN, 1, color);
	LEDMatrix__setRGBColor(LED_MATRIX_SIZE_LIN, LED_MATRIX_SIZE_COL, color);

	if (Buttons__isPressedOnce(&buttonDown))
	{
		FailureMemory__enterFault(FAILURE_ID__TEST);
	}

	if (Buttons__isPressed(&buttonUp))
	{
		if (failureTestTimer > 0)
		{
			failureTestTimer--;
		}
		else
		{
			failureTestTimer = 255;
			FailureMemory__clearAll();
		}
	}

	lineNb = failureCounter / LED_MATRIX_SIZE_COL;
	colNb = failureCounter % LED_MATRIX_SIZE_COL;

	for (linIt = 0; linIt < lineNb; linIt++)
	{
		for (colIt = 0; colIt < LED_MATRIX_SIZE_COL; colIt++)
		{
			if (FailureMemory__getErrortID(linIt * LED_MATRIX_SIZE_COL + colIt) == FAILURE_ID__WATCHDOG)
			{
				LEDMatrix__setRGBColorFromComponents(&color, 50, 0, 0);
			}
			else
			{
				LEDMatrix__setRGBColorFromComponents(&color, 0, 0, 50);
			}

			LEDMatrix__setRGBColor(linIt + 2, colIt + 1, color);
		}
	}

	for (colIt = 0; colIt < colNb; colIt++)
	{
		if (FailureMemory__getErrortID(lineNb * LED_MATRIX_SIZE_COL + colIt) == FAILURE_ID__WATCHDOG)
		{
			LEDMatrix__setRGBColorFromComponents(&color, 50, 0, 0);
		}
		else
		{
			LEDMatrix__setRGBColorFromComponents(&color, 0, 0, 50);
		}

		LEDMatrix__setRGBColor(lineNb + 2, colIt + 1, color);
	}
#endif
}
