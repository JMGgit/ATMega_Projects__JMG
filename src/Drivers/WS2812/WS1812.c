/*
 * WS1812.c
 *
 *  Created on: 16.01.2016
 *      Author: Jean-Martin George
 */


#include "WS2812.h"
#include "LEDMatrix.h"


#if (LED_TYPE == LED_TYPE_WS2812)

#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
static uint8_t RGBLedOrder;
static uint8_t RGBLedOrder_EEPROM EEMEM;
#endif
static uint8_t updateEnabled = TRUE;

#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIRECT)
RGB_Color_t GS_Data[LEDS_NB];
/* remark: cannot be declared as static: ATMega limitation?? */
#endif


uint8_t WS2812__getRGBLedOrder (void)
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
void WS2812__toggleRGBLedOrder (void)
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


#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIRECT)
static void WS2812__updateAll (void)
{
	ws2812_setleds((struct cRGB*)GS_Data, LEDS_NB);
}
#endif


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

	for (idxLed = 0; idxLed < LEDS_NB; idxLed++)
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
	WS2812_DigiDotBooster__setRGBForAllLEDs(LEDMatrix__getRGBColorFromComponents(0, 0, 0));
#endif
}


void WS2812__enableUpdate (uint8_t enable)
{
	updateEnabled = TRUE;
}


void WS2812__disableUpdate (uint8_t enable)
{
	updateEnabled = FALSE;
}


void WS2812__init (void)
{
#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
	RGBLedOrder = eeprom_read_byte(&RGBLedOrder_EEPROM);

	if (	(RGBLedOrder != RGB_LED_ORDER__BLUE_GREEN_RED)
		&& 	(RGBLedOrder != RGB_LED_ORDER__RED_GREEN_BLUE)
		)
	{
		RGBLedOrder = RGB_LED_ORDER__RED_GREEN_BLUE;
	}

#endif

#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIGIDOT_SPI)
	WS2812_DigiDotBooster__init();
#endif
}

#endif
