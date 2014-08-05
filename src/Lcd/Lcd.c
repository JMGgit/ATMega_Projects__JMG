/*
 * Lcd.c
 *
 *  Created on: 23.03.2014
 *      Author: Jean-Martin George
 */


#include "Lcd.h"
#include "../Clock/Clock.h"


#if (LCD_CONTROLLER != LCD_CONTROLLER_OFF)

static char LcdBuffer[LCD_MATRIX_SIZE_COL * LCD_MATRIX_SIZE_LIN];
static uint8_t cursorLine, cursorCol, refreshRequested;


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
#if (LCD_CONTROLLER == LCD_CONTROLLER_SPLC780D1)
	SPLC780D1__enableCursor();
#endif
	Lcd__setCursor(0, 0);
}


void Lcd__disableCursor (void)
{
#if (LCD_CONTROLLER == LCD_CONTROLLER_SPLC780D1)
	SPLC780D1__disableCursor();
#endif
}


void Lcd__setCursor (uint8_t line, uint8_t col)
{
	cursorLine = line;
	cursorCol = col;
}


void Lcd__requestRefresh (void)
{
	refreshRequested = TRUE;
}


void Lcd__x10 (void)
{
	static uint8_t updateCounter = 0;

	if ((refreshRequested == TRUE) || (updateCounter == 200))
	{
#if (LCD_CONTROLLER == LCD_CONTROLLER_SPLC780D1)
		SPLC780D1__setCursor(0);
		SPLC780D1__sendTable(LcdBuffer);
#endif
		updateCounter = 0;
		refreshRequested = FALSE;
#if (LCD_CONTROLLER == LCD_CONTROLLER_SPLC780D1)
		SPLC780D1__setCursorToLineCol(cursorLine, cursorCol);
#endif
	}
	else
	{
		updateCounter++;
	}
}

#endif
