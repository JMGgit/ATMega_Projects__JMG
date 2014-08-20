/*
 * LEDMatrix.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#include "LEDMatrix.h"
#include "LEDMatrix_DotCorrection.h"

#ifdef LED_MATRIX_SIZE_LIN

void LEDMatrix__setRGBColor (uint8_t line, uint8_t column, RGB_Color_t color)
{
	LEDMatrix__applyDotCorrection(&color, line, column);
	WS2801__setRGBForLED(color, LED_MATRIX_SIZE_COL * (line - 1) + (column - 1));
}

void LEDMatrix__clearMatrix (void)
{
	uint8_t linIt, colIt;

	for (linIt = 1; linIt <= LED_MATRIX_SIZE_LIN; linIt++)
	{
		for (colIt = 1; colIt <= LED_MATRIX_SIZE_COL; colIt++)
		{
			LEDMatrix__setRGBColor(linIt, colIt, getRGBColorFromComponents(0, 0, 0));
		}
	}

	for (colIt = 1; colIt <= 4; colIt++)
	{
		LEDMatrix__setRGBColor(LED_MATRIX_SIZE_LIN, LED_MATRIX_SIZE_COL + colIt, getRGBColorFromComponents(0, 0, 0));
	}
}

#endif
