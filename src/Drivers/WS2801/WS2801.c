/*
 * WS2801.c
 *
 *  Created on: 21.02.2013
 *      Author: Jean-Martin George
 */

#include "WS2801.h"

#ifdef WS2801_NB

RGB_Color_t GS_Data[WS2801_NB];
/* remark: cannot be declared as static: ATMega limitation?? */


void WS2801__x10 (void)
{
	WS2801__updateAll();
}


void WS2801__updateAll (void)
{
	uint16_t idxLed;

	for (idxLed = 0; idxLed < WS2801_NB; idxLed++)
	{
		/* copy data to register */
		SPDR = GS_Data[idxLed].blue;

		/* wait for transmission complete */
		while (!(SPSR & (1 << SPIF)))
		{
			;
		}

		SPDR = GS_Data[idxLed].green;

		/* wait for transmission complete */
		while (!(SPSR & (1 << SPIF)))
		{
			;
		}

		SPDR = GS_Data[idxLed].red;

		/* wait for transmission complete */
		while (!(SPSR & (1 << SPIF)))
		{
			;
		}
	}
}


void WS2801__setRGBForLED (RGB_Color_t color, uint16_t led)
{
	GS_Data[led] = color;
}


void WS2801__setRGBForAllLEDs (RGB_Color_t color)
{
	uint16_t idxLed;

	for (idxLed = 0; idxLed < WS2801_NB; idxLed++)
	{
		GS_Data[idxLed] = color;
	}
}


void WS2801__resetAllLEDs (void)
{
	memset(&GS_Data[0], 0, sizeof(GS_Data));
}

#endif
