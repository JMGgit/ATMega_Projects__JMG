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
	WS2801__setRGBForLED(color, (LED_MATRIX_SIZE_COL * (line - 1) + (column - 1)));
#endif
}


void LEDMatrix__setRGBColorForMatrix (RGB_Color_t color)
{
#ifdef WS2801_NB
	WS2801__setRGBForAllLEDs(color);
#endif
}


void LEDMatrix__clearMatrix (void)
{
#ifdef WS2801_NB
	WS2801__resetAllLEDs();
#endif
}


#endif
#endif
