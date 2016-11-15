/*
 * APA102.c
 *
 *  Created on: 27.10.2016
 *      Author: Jean-Martin George
 */


#include "APA102.h"


#if (LED_TYPE == LED_TYPE_APA102)
#include "LEDMatrix.h"

#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
static uint8_t RGBLedOrder;
static uint8_t RGBLedOrder_EEPROM EEMEM;
#endif
static uint8_t updateEnabled = TRUE;
static uint8_t globalBrightness;

#define START_FRAME_LENGTH	4
#if ((LEDS_NB % 16) == 0)
#define STOP_FRAME_LENGTH	(LEDS_NB / 16)
#else
#define STOP_FRAME_LENGTH	((LEDS_NB / 16) + 1)
#endif

RGB_Color_t GS_Data[LEDS_NB];
/* - cannot be declared as static: ATMega limitation??
 * - defined as struct to save run time compared to simple buffer */

static inline uint8_t APA102__getRGBLedOrder (void)
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
void APA102__toggleRGBLedOrder (void)
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


static void APA102__updateAll (void)
{
	/* SPI controlled directly instead of calling SPI functions to save runtime! */
	uint16_t idxLed, it;

	/* START FRAME*/
	for (it = 0; it < START_FRAME_LENGTH; it++)
	{
		SPDR = 0;
		while (!(SPSR & (1 << SPIF))) {};
	}

	/* LED FRAMES */
	for (idxLed = 0; idxLed < LEDS_NB; idxLed++)
	{
		/* global brightness */
		SPDR = 0xE0 | globalBrightness;
		while (!(SPSR & (1 << SPIF))) {};

		/* SPI register directly addressed to save run time */
		if (APA102__getRGBLedOrder() == RGB_LED_ORDER__BLUE_GREEN_RED)
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
		if (APA102__getRGBLedOrder() == RGB_LED_ORDER__BLUE_GREEN_RED)
		{
			SPDR = GS_Data[idxLed].red;
		}
		else
		{
			SPDR = GS_Data[idxLed].blue;
		}

		while (!(SPSR & (1 << SPIF))) {};
	}

	/* END FRAME */
	for (it = 0; it < 4 + 1; it++)
	{
		SPDR = 0xFF;
		while (!(SPSR & (1 << SPIF))) {};
	}
}


void APA102__x10 (void)
{
	if (updateEnabled)
	{
		APA102__updateAll();
	}
}


void APA102__setRGBForLED (RGB_Color_t color, uint16_t led)
{
	if (updateEnabled)
	{
		GS_Data[led] = color;
	}
}


void APA102__setRGBForAllLEDs (RGB_Color_t color)
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


void APA102__resetAllLEDs (void)
{
	if (updateEnabled)
	{
		memset(&GS_Data[0], 0, sizeof(GS_Data));
	}
}


void APA102__enableUpdate (uint8_t enable)
{
	updateEnabled = TRUE;
}


void APA102__disableUpdate (uint8_t enable)
{
	updateEnabled = FALSE;
}


void APA102__setGlobalBrightness (uint8_t brightness)
{
	if (brightness <= APA102_GLOBAL_BRIGHNESS__MAX)
	{
		globalBrightness = brightness;
	}
	else
	{
		globalBrightness = APA102_GLOBAL_BRIGHNESS__MAX;
	}
}


void APA102__init (void)
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

	APA102__setGlobalBrightness(APA102_GLOBAL_BRIGHNESS__MAX);
}

#endif
