/*
 * WS1812.c
 *
 *  Created on: 16.01.2016
 *      Author: Jean-Martin George
 */


#include "WS2812.h"


#ifdef WS2812_NB

RGB_Color_t GS_Data[WS2812_NB];
/* remark: cannot be declared as static: ATMega limitation?? */


static void WS2812__updateAll (void)
{
	ws2812_setleds((struct cRGB*)GS_Data, WS2812_NB);
}


void WS2812__x10 (void)
{
	WS2812__updateAll();
}


void WS2812__setRGBForLED (RGB_Color_t color, uint16_t led)
{
	GS_Data[led] = color;
}


void WS2812__setRGBForAllLEDs (RGB_Color_t color)
{
	uint16_t idxLed;

	for (idxLed = 0; idxLed < WS2812_NB; idxLed++)
	{
		GS_Data[idxLed] = color;
	}
}


void WS2812__resetAllLEDs (void)
{
	memset(&GS_Data[0], 0, sizeof(GS_Data));
}

#endif
