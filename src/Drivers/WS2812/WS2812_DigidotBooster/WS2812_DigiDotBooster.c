/*
 * WS2812_DigiDotBooster.c
 *
 *  Created on: 28.03.2016
 *      Author: Jean-Martin George
 */


#include "WS2812_DigiDotBooster.h"

#ifdef WS2812_NB
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIGIDOT_SPI)

#define TXBUFFERSIZE	10

#define CMD_INIT		0xB1
#define CMD_COUNT		WS2812_NB
#define CMD_BITS		24
#define CMD_SHOW		0xB2
#define CMD_SETRGB		0xA1
#define CMD_SETLED		0xA4
#define CMD_SETALL		0xA5

static uint8_t txBuffer[TXBUFFERSIZE];
static uint8_t txbufferIdx, globalBufferIdx;

/* store color to avoid too many data transmission to RGB register
 * (optimization may work for Qlocktwo but not for big matrix or if cells are independant) */
static RGB_Color_t lastColor;
static uint8_t slaveSelected = FALSE;


static inline void WS2812_DigiDotBooster__transmitTxBuffer (uint8_t finishTransmission)
{
	if (!slaveSelected)
	{
		SPI__slaveSelect(&WS2812_DIGIDOT_CS_DDR, &WS2812_DIGIDOT_CS_PORT, WS2812_DIGIDOT_CS_PIN, TRUE);
	}

	SPI__transmitData(txBuffer, txbufferIdx);

	txbufferIdx = 0;

	slaveSelected = TRUE;

	if (finishTransmission)
	{
		SPI__slaveSelect(&WS2812_DIGIDOT_CS_DDR, &WS2812_DIGIDOT_CS_PORT, WS2812_DIGIDOT_CS_PIN, FALSE);

		slaveSelected = FALSE;

		lastColor.red = 255;
		lastColor.green = 255;
		lastColor.blue = 255;

		globalBufferIdx = 0;
	}
}


static inline void WS2812_DigiDotBooster__addData (uint8_t data)
{
	txBuffer[txbufferIdx++] = data;
	globalBufferIdx++;

	if (globalBufferIdx == 255)
	{
		/* not allowed by DigiDitBooster! */
		while (1)
		{
			;
		}
	}
}


void WS2812_DigiDotBooster__init (void)
{
	txbufferIdx = 0;
	slaveSelected = 0;

	WS2812_DigiDotBooster__addData(CMD_INIT);
	WS2812_DigiDotBooster__addData(CMD_COUNT);
	WS2812_DigiDotBooster__addData(CMD_BITS);

	WS2812_DigiDotBooster__transmitTxBuffer(TRUE);
	_delay_ms(10);

	lastColor.red = 255;
	lastColor.green = 255;
	lastColor.blue = 255;
}


void WS2812_DigiDotBooster__setRGBForLED (RGB_Color_t color, uint8_t led)
{
	if (	(color.red 		!= lastColor.red)
		||	(color.green 	!= lastColor.green)
		|| 	(color.blue 	!= lastColor.blue)
		)
	{
		WS2812_DigiDotBooster__addData(CMD_SETRGB);
		WS2812_DigiDotBooster__addData(color.red);
		WS2812_DigiDotBooster__addData(color.green);
		WS2812_DigiDotBooster__addData(color.blue);

		lastColor.red 	= color.red;
		lastColor.green = color.green;
		lastColor.blue 	= color.blue;
	}

	WS2812_DigiDotBooster__addData(CMD_SETLED);
	WS2812_DigiDotBooster__addData(led);

	WS2812_DigiDotBooster__transmitTxBuffer(FALSE);
}

void WS2812_DigiDotBooster__setRGBForAllLEDs (RGB_Color_t color)
{
	if (	(color.red 		!= lastColor.red)
		||	(color.green 	!= lastColor.green)
		|| 	(color.blue 	!= lastColor.blue)
		)
	{
		WS2812_DigiDotBooster__addData(CMD_SETRGB);
		WS2812_DigiDotBooster__addData(color.red);
		WS2812_DigiDotBooster__addData(color.green);
		WS2812_DigiDotBooster__addData(color.blue);

		lastColor.red 	= color.red;
		lastColor.green = color.green;
		lastColor.blue 	= color.blue;
	}

	WS2812_DigiDotBooster__addData(CMD_SETALL);

	WS2812_DigiDotBooster__transmitTxBuffer(FALSE);
}


void WS2812_DigiDotBooster__x10 (void)
{
	/* for some reason the command SHOW cannot be sent alone?? -> set RGB value before */
	WS2812_DigiDotBooster__addData(CMD_SETRGB);
	WS2812_DigiDotBooster__addData(0);
	WS2812_DigiDotBooster__addData(0);
	WS2812_DigiDotBooster__addData(0);

	WS2812_DigiDotBooster__addData(CMD_SHOW);

	WS2812_DigiDotBooster__transmitTxBuffer(TRUE);
}

#endif
#endif
