/*
 * SPLC780D1.c
 *
 *  Created on: 22.03.2014
 *      Author: Jean-Martin George
 */


#include "SPLC780D1.h"

#define DELAY_POWER_ON_MS		15
#define DELAY_INIT_1_MS			5
#define DELAY_INIT_2_MS			2

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
	_delay_ms(DELAY_INIT_2_MS);
	SPLC780D1__sendCommand(CMD_DISPLAY_SET | SET_BLINKING_OFF | SET_CURSOR_OFF | SET_DISPLAY_ON);
	SPLC780D1__sendCommand(CMD_CLEAR_DISPLAY);
	_delay_ms(DELAY_INIT_2_MS);
	SPLC780D1__sendCommand(CMD_ENTRY_MODE_SET | SET_CURSOR_INCREMENT | SET_SCROLL_OFF);
	_delay_ms(DELAY_INIT_2_MS);
	SPLC780D1__sendCommand(CMD_RETURN_HOME);
	_delay_ms(DELAY_INIT_2_MS);

	char testBuffer[81];
	strcpy(testBuffer, "Salut Schissi, je   crois que ca marche pas trop mal le LCD! Pouet A++          ");

	SPLC780D1__sendTable(testBuffer);
}


static void SPLC780D1__write (uint8_t data, uint8_t RS)
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
	_delay_ms(DELAY_INIT_2_MS);
}


void SPLC780D1__sendTable (uint8_t *data)
{
	uint8_t i;

	for (i = 0; i < 80; i++)
	{
		if (i < 20)
		{
			SPLC780D1__sendData(data[i]);
		}
		else if (i < 40)
		{
			SPLC780D1__sendData(data[i + 20]);
		}
		else if (i < 60)
		{
			SPLC780D1__sendData(data[i - 20]);
		}
		else
		{
			SPLC780D1__sendData(data[i]);
		}
	}
}

void SPLC780D1__sendData (uint8_t data)
{
	SPLC780D1__write(data, TRUE);
}


void SPLC780D1__sendCommand (uint8_t data)
{
	SPLC780D1__write(data, FALSE);
}


void SPLC780D1__x10 (void)
{

}
