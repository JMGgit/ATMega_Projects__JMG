/*
 * LEDMatrix.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#include "LEDMatrix.h"

#ifdef LED_MATRIX_SIZE_LIN
#ifdef LED_MATRIX_SIZE_COL


void LEDMatrix__setRGBColor (uint8_t line, uint8_t column, RGB_Color_t color)
{
	uint16_t ledPosition;

	LEDMatrix__applyDotCorrection(&color, line, column);
	ledPosition = LED_MATRIX_SIZE_COL * (line - 1) + (column - 1);

#if (LED_TYPE == LED_TYPE_WS2801)
	WS2801__setRGBForLED(color, ledPosition);
#endif
#if (LED_TYPE == LED_TYPE_WS2812)
	WS2812__setRGBForLED(color, ledPosition);
#endif
}


void LEDMatrix__setRGBColorForMatrix (RGB_Color_t color)
{
#if (LED_TYPE == LED_TYPE_WS2801)
	WS2801__setRGBForAllLEDs(color);
#endif
#if (LED_TYPE == LED_TYPE_WS2812)
	WS2812__setRGBForAllLEDs(color);
#endif
}


void LEDMatrix__clearMatrix (void)
{
#if (LED_TYPE == LED_TYPE_WS2801)
	WS2801__resetAllLEDs();
#endif
#if (LED_TYPE == LED_TYPE_WS2812)
	WS2812__resetAllLEDs();
#endif
}


void LEDMatrix__toggleledOrder (void)
{
#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
#if (LED_TYPE == LED_TYPE_WS2801)
	WS2801__toggleledOrder();
#endif
#if (LED_TYPE == LED_TYPE_WS2812)
	WS2812__toggleledOrder();
#endif
#endif
}


#endif
#endif
