/*
 * WS2801.c
 *
 *  Created on: 21.02.2013
 *      Author: Jean-Martin George
 */

#include "WS2801.h"

#ifdef WS2801_NB

static uint8_t GS_Data_0[256];
static uint8_t GS_Data_1[256];
static uint8_t GS_Data_2[256];
static uint8_t GS_Data_3[256];


void WS2801__x10 (void)
{
	WS2801__updateAll();
}


void WS2801__updateAll (void)
{
	uint16_t itChannel;

	for (itChannel = 0; itChannel < CHANNEL_NB; itChannel++)
	{
		if (itChannel < 256)
		{
			SPI__transmitDataBytes(&GS_Data_0[itChannel], 1);
		}
		else if (itChannel < 512)
		{
			SPI__transmitDataBytes(&GS_Data_1[itChannel - 256], 1);
		}
		else if (itChannel < 768)
		{
			SPI__transmitDataBytes(&GS_Data_2[itChannel - 512], 1);
		}
		else if (itChannel < 1024)
		{
			SPI__transmitDataBytes(&GS_Data_3[itChannel - 768], 1);
		}
		else
		{
			/* not supported for now */
		}
	}
}


void WS2801__setGSForChannel (uint8_t gsData, uint16_t channel)
{
	if (channel < CHANNEL_NB)
	{
		if (channel < 256)
		{
			GS_Data_0[channel] = gsData;
		}
		else if (channel < 512)
		{
			GS_Data_1[channel - 256] = gsData;
		}
		else if (channel < 768)
		{
			GS_Data_2[channel - 512] = gsData;
		}
		else
		{
			GS_Data_3[channel - 768] = gsData;
		}
	}
}


void WS2801__setRGBForLED (RGB_Color_t color, uint16_t led)
{
#if (RGB_LED_CONNECTION == RGB_LED_CONNECTION__BLUE_GREEN_RED)
		WS2801__setGSForChannel(color.red, led * 3 + 2);
		WS2801__setGSForChannel(color.green, led * 3 + 1);
		WS2801__setGSForChannel(color.blue, led * 3);
#elif (RGB_LED_CONNECTION == RGB_LED_CONNECTION__RED_GREEN_BLUE)
		WS2801__setGSForChannel(color.red, led * 3);
		WS2801__setGSForChannel(color.green, led * 3 + 1);
		WS2801__setGSForChannel(color.blue, led * 3 + 2);
#endif
}

#endif
