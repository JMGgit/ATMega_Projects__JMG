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
static uint8_t RGBLedOrder;
static uint8_t RGBLedOrder_EEPROM EEMEM;
#endif
static uint8_t updateEnabled = TRUE;

RGB_Color_t GS_Data[LEDS_NB];
/* - cannot be declared as static: ATMega limitation??
 * - defined as struct to save run time compared to simple buffer */

static inline uint8_t WS2801__getRGBLedOrder (void)
{
#if (RGB_LED_ORDER == RGB_LED_ORDER__RED_GREEN_BLUE)
	return RGB_LED_ORDER__RED_GREEN_BLUE;
#elif (RGB_LED_ORDER == RGB_LED_ORDER__BLUE_GREEN_RED)
	return RGB_LED_ORDER__BLUE_GREEN_RED;
#else
	return RGBLedOrder;
#endif
}


#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
void WS2801__toggleRGBLedOrder (void)
{
	if (RGBLedOrder == RGB_LED_ORDER__RED_GREEN_BLUE)
	{
		RGBLedOrder = RGB_LED_ORDER__BLUE_GREEN_RED;
	}
	else
	{
		RGBLedOrder = RGB_LED_ORDER__RED_GREEN_BLUE;
	}

	eeprom_update_byte(&RGBLedOrder_EEPROM, RGBLedOrder);
}
#endif


static void WS2801__updateAll (void)
{
	/* SPI controlled directly instead of calling SPI functions to save runtime! */
	uint16_t idxLed;

	for (idxLed = 0; idxLed < LEDS_NB; idxLed++)
	{
		/* SPI register directly addressed to save run time */
		if (WS2801__getRGBLedOrder() == RGB_LED_ORDER__BLUE_GREEN_RED)
		{
			SPDR = GS_Data[idxLed].blue;
		}
		else
		{
			SPDR = GS_Data[idxLed].red;
		}

		while (!(SPSR & (1 << SPIF))) {};
		SPDR = GS_Data[idxLed].green;
		while (!(SPSR & (1 << SPIF))) {};

		/* copy data to register */
		if (WS2801__getRGBLedOrder() == RGB_LED_ORDER__BLUE_GREEN_RED)
		{
			SPDR = GS_Data[idxLed].red;
		}
		else
		{
			SPDR = GS_Data[idxLed].blue;
		}

		/* wait for transmission complete */
		while (!(SPSR & (1 << SPIF))) {};
	}
}


void WS2801__x10 (void)
{
	if (updateEnabled)
	{
		WS2801__updateAll();
	}
}


void WS2801__setRGBForLED (RGB_Color_t color, uint16_t led)
{
	if (updateEnabled)
	{
		GS_Data[led] = color;
	}
}


void WS2801__setRGBForAllLEDs (RGB_Color_t color)
{
	uint16_t idxLed;

	if (updateEnabled)
	{
		for (idxLed = 0; idxLed < LEDS_NB; idxLed++)
		{
			GS_Data[idxLed] = color;
		}
	}
}


void WS2801__resetAllLEDs (void)
{
	if (updateEnabled)
	{
		memset(&GS_Data[0], 0, sizeof(GS_Data));
	}
}


void WS2801__enableUpdate (uint8_t enable)
{
	updateEnabled = TRUE;
}


void WS2801__disableUpdate (uint8_t enable)
{
	updateEnabled = FALSE;
}


void WS2801__init (void)
{
#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
	RGBLedOrder = eeprom_read_byte(&RGBLedOrder_EEPROM);

	if (	(RGBLedOrder != RGB_LED_ORDER__BLUE_GREEN_RED)
		&& 	(RGBLedOrder != RGB_LED_ORDER__RED_GREEN_BLUE)
		)
	{
		RGBLedOrder = RGB_LED_ORDER__RED_GREEN_BLUE;
		eeprom_update_byte(&RGBLedOrder_EEPROM, RGBLedOrder);
	}

#endif
}

#endif
