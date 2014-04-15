/*
 * Lcd.c
 *
 *  Created on: 23.03.2014
 *      Author: Jean-Martin George
 */


#include "Lcd.h"
#include "../Clock/Clock.h"


static char LcdBuffer[LCD_MATRIX_SIZE_COL * LCD_MATRIX_SIZE_LIN];
static uint8_t cursorLine, cursorCol;


void Lcd__init (void)
{
	strcpy(LcdBuffer, "                                                                                ");
}


void Lcd__writeLine (char* data, uint8_t line)
{
	memcpy(&LcdBuffer[(line - 1) * LCD_MATRIX_SIZE_COL], data, LCD_MATRIX_SIZE_COL);
}


void Lcd__newLine (char* line)
{
	uint8_t i;

	for (i = 0; i < LCD_MATRIX_SIZE_COL; i++)
	{
		line[i] = ' ';
	}
}


void Lcd__enableCursor (void)
{
	SPLC780D1__enableCursor();
	Lcd__setCursor(0, 0);
}


void Lcd__disableCursor (void)
{
	SPLC780D1__disableCursor();
}


void Lcd__setCursor (uint8_t line, uint8_t col)
{
	cursorLine = line;
	cursorCol = col;
}


void Lcd__x10 (void)
{
	static uint8_t updateCounter = 0;

	if (updateCounter < 20)
	{
		updateCounter++;
	}
	else
	{
		SPLC780D1__setCursor(0);
		SPLC780D1__sendTable(LcdBuffer);
		updateCounter = 0;
		SPLC780D1__setCursorToLineCol(cursorLine, cursorCol);
	}
}
