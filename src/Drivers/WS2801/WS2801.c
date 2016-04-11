/*
 * WS2801.c
 *
 *  Created on: 21.02.2013
 *      Author: Jean-Martin George
 */

#include "WS2801.h"
#include "LEDMatrix.h"


#if (LED_TYPE == LED_TYPE_WS2801)

#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
static uint8_t ledOrder;
static uint8_t ledOrder_EEPROM EEMEM;
#endif

RGB_Color_t GS_Data[LEDS_NB];
/* remark: cannot be declared as static: ATMega limitation?? */


static inline uint8_t WS2801__getledOrder (void)
{
#if (RGB_LED_ORDER == RGB_LED_ORDER__RED_GREEN_BLUE)
	return RGB_LED_ORDER__RED_GREEN_BLUE;
#elif (RGB_LED_ORDER == RGB_LED_ORDER__BLUE_GREEN_RED)
	return RGB_LED_ORDER__BLUE_GREEN_RED;
#else
	return ledOrder;
#endif
}


#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
void WS2801__toggleledOrder (void)
{
	if (ledOrder == RGB_LED_ORDER__RED_GREEN_BLUE)
	{
		ledOrder = RGB_LED_ORDER__BLUE_GREEN_RED;
	}
	else
	{
		ledOrder = RGB_LED_ORDER__RED_GREEN_BLUE;
	}

	eeprom_update_byte(&ledOrder_EEPROM, ledOrder);
}
#endif


static void WS2801__updateAll (void)
{
	/* SPI controlled directly instead of calling SPI functions to save runtime! */
	uint16_t idxLed;

	for (idxLed = 0; idxLed < LEDS_NB; idxLed++)
	{
		/* copy data to register */
		if (WS2801__getledOrder() == RGB_LED_ORDER__BLUE_GREEN_RED)
		{
			SPDR = GS_Data[idxLed].blue;
		}
		else
		{
			SPDR = GS_Data[idxLed].red;
		}

		/* wait for transmission complete */
		while (!(SPSR & (1 << SPIF)))
		{
			;
		}

		/* copy data to register */
		SPDR = GS_Data[idxLed].green;

		/* wait for transmission complete */
		while (!(SPSR & (1 << SPIF)))
		{
			;
		}

		/* copy data to register */
		if (WS2801__getledOrder() == RGB_LED_ORDER__BLUE_GREEN_RED)
		{
			SPDR = GS_Data[idxLed].red;
		}
		else
		{
			SPDR = GS_Data[idxLed].blue;
		}

		/* wait for transmission complete */
		while (!(SPSR & (1 << SPIF)))
		{
			;
		}
	}
}


void WS2801__x10 (void)
{
	WS2801__updateAll();
}


void WS2801__setRGBForLED (RGB_Color_t color, uint16_t led)
{
	GS_Data[led] = color;
}


void WS2801__setRGBForAllLEDs (RGB_Color_t color)
{
	uint16_t idxLed;

	for (idxLed = 0; idxLed < LEDS_NB; idxLed++)
	{
		GS_Data[idxLed] = color;
	}
}


void WS2801__resetAllLEDs (void)
{
	memset(&GS_Data[0], 0, sizeof(GS_Data));
}


void WS2801__init (void)
{
#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
	ledOrder = eeprom_read_byte(&ledOrder_EEPROM);

	if (	(ledOrder != RGB_LED_ORDER__BLUE_GREEN_RED)
		&& 	(ledOrder != RGB_LED_ORDER__RED_GREEN_BLUE)
		)
	{
		ledOrder = RGB_LED_ORDER__RED_GREEN_BLUE;
	}

#endif
}

#endif
