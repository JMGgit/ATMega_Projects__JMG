/*
 * WS1812.c
 *
 *  Created on: 16.01.2016
 *      Author: Jean-Martin George
 */


#include "WS2812.h"


#ifdef WS2812_NB


#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIRECT)
RGB_Color_t GS_Data[WS2812_NB];
/* remark: cannot be declared as static: ATMega limitation?? */
#endif


#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIRECT)
static void WS2812__updateAll (void)
{
	ws2812_setleds((struct cRGB*)GS_Data, WS2812_NB);

}
#endif

void WS2812__init (void)
{
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIGIDOT_SPI)
	WS2812_DigiDotBooster__init();
#endif
}


void WS2812__x10 (void)
{
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIRECT)
	WS2812__updateAll();
#endif
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIGIDOT_SPI)
	WS2812_DigiDotBooster__x10();
#endif
}


void WS2812__setRGBForLED (RGB_Color_t color, uint16_t led)
{
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIRECT)
	GS_Data[led] = color;
#endif
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIGIDOT_SPI)
	WS2812_DigiDotBooster__setRGBForLED(color, led);
#endif
}


void WS2812__setRGBForAllLEDs (RGB_Color_t color)
{
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIRECT)
	uint16_t idxLed;

	for (idxLed = 0; idxLed < WS2812_NB; idxLed++)
	{
		GS_Data[idxLed] = color;
	}
#endif
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIGIDOT_SPI)
	WS2812_DigiDotBooster__setRGBForAllLEDs(color);
#endif
}


void WS2812__resetAllLEDs (void)
{
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIRECT)
	memset(&GS_Data[0], 0, sizeof(GS_Data));
#endif
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIGIDOT_SPI)
	WS2812_DigiDotBooster__setRGBForAllLEDs(getRGBColorFromComponents(0, 0, 0));
#endif
}

#endif
