/*
 * Lcd.c
 *
 *  Created on: 23.03.2014
 *      Author: Jean-Martin George
 */


#include "Lcd.h"
#include "../Clock/Clock.h"


static uint8_t LcdBuffer[LCD_MATRIX_SIZE_COL * LCD_MATRIX_SIZE_LIN];
static uint8_t LcdLine[LCD_MATRIX_SIZE_COL];


void Lcd__init (void)
{
	strcpy(LcdBuffer, "                                                                                ");
	strcpy(LcdLine, "                    ");
}


void Lcd__writeLine (uint8_t *data, uint8_t line)
{
	memcpy(&LcdBuffer[(line - 1) * LCD_MATRIX_SIZE_COL], data, LCD_MATRIX_SIZE_COL);
}


void Lcd__x10 (void)
{
	static uint8_t updateCounter = 0;

	uint8_t hours, minutes, seconds;
	uint8_t idxHours, idxMinutes, idxSeconds;

	hours = Clock__getHours();
	minutes = Clock__getMinutes();
	seconds = Clock__getSeconds();

	if (hours > 10)
	{
		itoa(hours, &LcdLine[6], 10);
	}
	else
	{
		LcdLine[8] = '0';
		itoa(hours, &LcdLine[7], 10);
	}

	if (minutes > 10)
	{
		itoa(minutes, &LcdLine[9], 10);
	}
	else
	{
		LcdLine[9] = '0';
		itoa(minutes, &LcdLine[10], 10);
	}

	if (seconds > 10)
	{
		itoa(seconds, &LcdLine[12], 10);
	}
	else
	{
		LcdLine[12] = '0';
		itoa(seconds, &LcdLine[13], 10);
	}

	LcdLine[8] = ':';
	LcdLine[11] = ':';
	LcdLine[14] = ' ';

	Lcd__writeLine(LcdLine, 2);

	if (updateCounter < 10)
	{
		updateCounter++;
	}
	else
	{
		SPLC780D1__sendTable(LcdBuffer);
		updateCounter = 0;
	}
}
