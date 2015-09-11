/*
 * LEDMatrix.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#include "LEDMatrix.h"
#include "LEDMatrix_DotCorrection.h"


#ifdef LED_MATRIX_SIZE_LIN
#ifdef LED_MATRIX_SIZE_COL

void LEDMatrix__setRGBColor (uint8_t line, uint8_t column, RGB_Color_t color)
{
	LEDMatrix__applyDotCorrection(&color, line, column);
#ifdef WS2801_NB
	WS2801__setRGBForLED(color, LED_MATRIX_SIZE_COL * (line - 1) + (column - 1));
#endif
}

void LEDMatrix__clearMatrix (void)
{
#ifdef WS2801_NB
	WS2801__resetAllLEDs();
#endif
#if (PROJECT == PROJECT__QLOCKTWO)
	uint8_t colIt;

	/* clear 4 edges */
	for (colIt = 1; colIt <= 4; colIt++)
	{
		LEDMatrix__setRGBColor(LED_MATRIX_SIZE_LIN, LED_MATRIX_SIZE_COL + colIt, getRGBColorFromComponents(0, 0, 0));
	}
#endif
}

#endif
#endif
