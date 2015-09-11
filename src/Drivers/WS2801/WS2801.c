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
	if (CHANNEL_NB < 256)
	{
		SPI__transmitData(GS_Data_0, CHANNEL_NB);
	}
	else if (CHANNEL_NB < 512)
	{
		SPI__transmitData(GS_Data_0, 256);
		SPI__transmitData(GS_Data_1, CHANNEL_NB - 256);
	}
	else if (CHANNEL_NB < 768)
	{
		SPI__transmitData(GS_Data_0, 256);
		SPI__transmitData(GS_Data_1, 256);
		SPI__transmitData(GS_Data_2, CHANNEL_NB - 512);
	}
	else
	{
		SPI__transmitData(GS_Data_0, 256);
		SPI__transmitData(GS_Data_1, 256);
		SPI__transmitData(GS_Data_2, 256);
		SPI__transmitData(GS_Data_3, CHANNEL_NB - 768);
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


void WS2801__resetAllLEDs (void)
{
	if (CHANNEL_NB < 256)
	{
		memset(GS_Data_0, 0, CHANNEL_NB);
	}
	else if (CHANNEL_NB < 512)
	{
		memset(GS_Data_0, 0, 256);
		memset(GS_Data_1, 0, CHANNEL_NB - 256);
	}
	else if (CHANNEL_NB < 768)
	{
		memset(GS_Data_0, 0, 256);
		memset(GS_Data_1, 0, 256);
		memset(GS_Data_2, 0, CHANNEL_NB - 512);
	}
	else
	{
		memset(GS_Data_0, 0, 256);
		memset(GS_Data_1, 0, 256);
		memset(GS_Data_2, 0, 256);
		memset(GS_Data_3, 0, CHANNEL_NB - 768);
	}
}

#endif
