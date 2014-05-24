/*
 * SPLC780D1.c
 *
 *  Created on: 22.03.2014
 *      Author: Jean-Martin George
 */


#include "SPLC780D1.h"


#if (LCD_CONTROLLER == LCD_CONTROLLER_SPLC780D1)

#define DELAY_POWER_ON_MS		15
#define DELAY_INIT_1_MS			5
#define DELAY_INIT_2_MS			2
#define DELAY_WRITE_US			500

#define CMD_CLEAR_DISPLAY		0x01
#define CMD_RETURN_HOME			0x02

#define CMD_ENTRY_MODE_SET		0x04
#define SET_CURSOR_DECREMENT	0x00
#define SET_SCROLL_OFF			0x00
#define SET_SCROLL_ON			0x01
#define SET_CURSOR_INCREMENT	0x02

#define CMD_DISPLAY_SET			0x08
#define SET_BLINKING_OFF		0x00
#define SET_BLINKING_ON			0x01
#define SET_CURSOR_OFF			0x00
#define SET_CURSOR_ON			0x02
#define SET_DISPLAY_OFF			0x00
#define SET_DISPLAY_ON			0x04

#define CMD_FUNCTION_SET 		0x20
#define SET_DL_8_BIT			0x10
#define SET_DL_4_BIT			0x00
#define SET_DISP_2_LINES		0x08
#define SET_DISP_1_LINE			0x00
#define SET_FONT_BIG			0x04
#define SET_FONT_SMALL			0x00

#define CMD_SET_DDRAM_DATA		0x80


void SPLC780D1__init (void)
{
	setOutput(LCD_RS_DDR, LCD_RS_PIN);
	setOutput(LCD_RW_DDR, LCD_RW_PIN);
	setOutput(LCD_E_DDR, LCD_E_PIN);
	LCD_DB_DDR = 0xFF;

	 _delay_ms(16);
	 SPLC780D1__sendCommand(CMD_FUNCTION_SET | SET_DL_8_BIT);
	_delay_ms(DELAY_INIT_1_MS);
	SPLC780D1__sendCommand(CMD_FUNCTION_SET | SET_DL_8_BIT);
	 _delay_us(200);
	SPLC780D1__sendCommand(CMD_FUNCTION_SET | SET_DL_8_BIT | SET_DISP_2_LINES | SET_FONT_BIG);
	SPLC780D1__sendCommand(CMD_DISPLAY_SET | SET_BLINKING_OFF | SET_CURSOR_OFF | SET_DISPLAY_ON);
	SPLC780D1__sendCommand(CMD_CLEAR_DISPLAY);
	SPLC780D1__sendCommand(CMD_ENTRY_MODE_SET | SET_CURSOR_INCREMENT | SET_SCROLL_OFF);
	SPLC780D1__sendCommand(CMD_RETURN_HOME);
}


static void SPLC780D1__write (char data, uint8_t RS)
{
	if (RS == TRUE)
	{
		setHigh(LCD_RS_PORT, LCD_RS_PIN);
	}
	else
	{
		setLow(LCD_RS_PORT, LCD_RS_PIN);
	}

	setLow(LCD_RW_PORT, LCD_RW_PIN);
	setHigh(LCD_E_PORT, LCD_E_PIN);
	LCD_DB_PORT = data;
	setLow(LCD_E_PORT, LCD_E_PIN);
	setHigh(LCD_RW_PORT, LCD_RW_PIN);
	_delay_us(DELAY_WRITE_US);
}


void SPLC780D1__sendTable (char *data)
{
	uint8_t i;

	for (i = 0; i < (LCD_MATRIX_SIZE_COL * LCD_MATRIX_SIZE_LIN); i++)
	{
		if (i < LCD_MATRIX_SIZE_COL)
		{
			SPLC780D1__sendData(data[i]);
		}
		else if (i < (2 * LCD_MATRIX_SIZE_COL))
		{
			SPLC780D1__sendData(data[i + 20]);
		}
		else if (i < (3* LCD_MATRIX_SIZE_COL))
		{
			SPLC780D1__sendData(data[i - 20]);
		}
		else
		{
			SPLC780D1__sendData(data[i]);
		}
	}
}


void SPLC780D1__sendData (char data)
{
	SPLC780D1__write(data, TRUE);
}


void SPLC780D1__sendCommand (char data)
{
	SPLC780D1__write(data, FALSE);
}


void SPLC780D1__sendLine (char data, uint8_t line)
{
	uint8_t cursorPos = 0;

	if (line == 1)
	{
		cursorPos = 0;
	}
	else if (line == 2)
	{
		cursorPos = 64;
	}
	else if (line == 3)
	{
		cursorPos = LCD_MATRIX_SIZE_COL;
	}
	else if (line == 4)
	{
		cursorPos = 64 + LCD_MATRIX_SIZE_COL;
	}

	SPLC780D1__setCursor(cursorPos);
	SPLC780D1__sendData(data);
}

void SPLC780D1__enableCursor (void)
{
	SPLC780D1__sendCommand(CMD_DISPLAY_SET | SET_BLINKING_ON | SET_CURSOR_OFF | SET_DISPLAY_ON);
}


void SPLC780D1__disableCursor (void)
{
	SPLC780D1__sendCommand(CMD_DISPLAY_SET | SET_BLINKING_OFF | SET_CURSOR_OFF | SET_DISPLAY_ON);
}


void SPLC780D1__setCursor (uint8_t position)
{
	SPLC780D1__sendCommand(CMD_SET_DDRAM_DATA | position);
}


void SPLC780D1__setCursorToLineCol (uint8_t line, uint8_t col)
{
	uint8_t cursorPos = 0;

	if (line == 1)
	{
		cursorPos = col - 1;
	}
	else if (line == 2)
	{
		cursorPos = 64 + (col - 1);
	}
	else if (line == 3)
	{
		cursorPos = LCD_MATRIX_SIZE_COL + col - 1;
	}
	else if (line == 4)
	{
		cursorPos = 64 + LCD_MATRIX_SIZE_COL + col - 1;
	}

	SPLC780D1__setCursor(cursorPos);
}

#endif
