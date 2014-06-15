/*
 * Mode_Transfer.c
 *
 *  Created on: 10.06.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Transfer.h"


#define SEL_BACK		1


static uint8_t currentSelectedState;
static uint8_t previousSelectedState;
static uint8_t refresh;


void Mode_Transfer__init (void)
{
	Lcd__enableCursor();
	currentSelectedState = SEL_BACK;
	previousSelectedState = 0xFF;
	Transfer__init();
}


void Mode_Transfer__x10 (void)
{
	char lcdLine_1[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_2[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_3[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_4[LCD_MATRIX_SIZE_COL + 1];

	Transfer__x10();

	Lcd__newLine(lcdLine_1);
	Lcd__newLine(lcdLine_2);
	Lcd__newLine(lcdLine_3);
	Lcd__newLine(lcdLine_4);

	/* line 1 */
	strcpy(&lcdLine_1[0], "Transfert de donnees");

	/* line 4 */
	strcpy(&lcdLine_4[0], "<RETOUR>            ");

	Lcd__writeLine(lcdLine_1, 1);
	Lcd__writeLine(lcdLine_2, 2);
	Lcd__writeLine(lcdLine_3, 3);
	Lcd__writeLine(lcdLine_4, 4);

	if (refresh == TRUE)
	{
		Lcd__requestRefresh();
		refresh = FALSE;
	}

	switch (currentSelectedState)
	{
		case SEL_BACK:
		{
			Lcd__setCursor(4, 3);

			break;
		}
	}

	if (previousSelectedState != currentSelectedState)
	{
		refresh = TRUE;
		previousSelectedState = currentSelectedState;
	}
}


uint8_t Modes__transferToStandby (void)
{
	return ((currentSelectedState == SEL_BACK) && (Buttons__isPressedOnce(&buttonMode)));
}
