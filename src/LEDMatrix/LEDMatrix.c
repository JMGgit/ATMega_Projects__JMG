/*
 * LEDMatrix.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#include "LEDMatrix.h"

#ifdef LED_MATRIX_SIZE_LIN
#ifdef LED_MATRIX_SIZE_COL


#if (LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
static uint8_t ledOrder;
static uint8_t ledOrder_EEPROM EEMEM;
#endif


void LEDMatrix__setRGBColor (uint8_t line, uint8_t column, RGB_Color_t color)
{
	uint16_t ledPosition;

	LEDMatrix__applyDotCorrection(&color, line, column);

	if (LEDMatrix__getLedOrder() == LED_ORDER__STRAIGHT_FORWARD)
	{
		if (((line % 2) == 0) && (column <= LED_MATRIX_SIZE_COL))
		{
			/* column overflow allowed for Qlocktwo -> not considered here */
			column = (LED_MATRIX_SIZE_COL + 1) - column;
		}
	}

	ledPosition = LED_MATRIX_SIZE_COL * (line - 1) + (column - 1);

#if (LED_TYPE == LED_TYPE_WS2801)
	WS2801__setRGBForLED(color, ledPosition);
#endif
#if (LED_TYPE == LED_TYPE_WS2812)
	WS2812__setRGBForLED(color, ledPosition);
#endif
#if (LED_TYPE == LED_TYPE_APA102)
	APA102__setRGBForLED(color, ledPosition);
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
#if (LED_TYPE == LED_TYPE_APA102)
	APA102__setRGBForAllLEDs(color);
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
#if (LED_TYPE == LED_TYPE_APA102)
	APA102__resetAllLEDs();
#endif
}


void LEDMatrix__enableUpdate (void)
{
#if (LED_TYPE == LED_TYPE_WS2801)
	WS2801__enableUpdate(TRUE);
#endif
#if (LED_TYPE == LED_TYPE_WS2812)
	WS2812__enableUpdate(TRUE);
#endif
#if (LED_TYPE == LED_TYPE_APA102)
	APA102__enableUpdate(TRUE);
#endif
}


void LEDMatrix__disableUpdate (void)
{
#if (LED_TYPE == LED_TYPE_WS2801)
	WS2801__disableUpdate(FALSE);
#endif
#if (LED_TYPE == LED_TYPE_WS2812)
	WS2812__disableUpdate(FALSE);
#endif
#if (LED_TYPE == LED_TYPE_APA102)
	APA102__disableUpdate(FALSE);
#endif
}


void LEDMatrix__toggleRGBLedOrder (void)
{
#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
#if (LED_TYPE == LED_TYPE_WS2801)
	WS2801__toggleRGBLedOrder();
#endif
#if (LED_TYPE == LED_TYPE_WS2812)
	WS2812__toggleRGBLedOrder();
#endif
#if (LED_TYPE == LED_TYPE_APA102)
	APA102__toggleRGBLedOrder();
#endif
#endif
}


void LEDMatrix__toggleLedOrder (void)
{
#if (LED_ORDER == LED_ORDER__CONFIGURABLE)
	if (ledOrder == LED_ORDER__LEFT_2_RIGHT)
	{
		ledOrder = LED_ORDER__STRAIGHT_FORWARD;
	}
	else
	{
		ledOrder = LED_ORDER__LEFT_2_RIGHT;
	}

	eeprom_update_byte(&ledOrder_EEPROM, ledOrder);
#endif
}


uint8_t LEDMatrix__getLedOrder (void)
{
#if (LED_ORDER == LED_ORDER__LEFT_2_RIGHT)
	return LED_ORDER__LEFT_2_RIGHT;
#elif (LED_ORDER == LED_ORDER__STRAIGHT_FORWARD)
	return LED_ORDER__STRAIGHT_FORWARD;
#else
	return ledOrder;
#endif
}


void LEDMatrix__init (void)
{
#if (LED_ORDER == LED_ORDER__CONFIGURABLE)
	ledOrder = eeprom_read_byte(&ledOrder_EEPROM);

	if (	(ledOrder != LED_ORDER__LEFT_2_RIGHT)
		&& 	(ledOrder != LED_ORDER__STRAIGHT_FORWARD)
		)
	{
		ledOrder = LED_ORDER__LEFT_2_RIGHT;
		eeprom_update_byte(&ledOrder_EEPROM, ledOrder);
	}
#endif
}

#endif
#endif
