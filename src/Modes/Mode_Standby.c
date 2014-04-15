/*
 * Mode_Standby.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Standby.h"


#define SEL_SETUP		0
#define	SEL_START		1

static uint8_t selectState = SEL_SETUP;


void Mode_Standby__init (void)
{
	Lcd__enableCursor();
}

void Mode_Standby__x10 (void)
{
	char lcdLine_1[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_2[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_3[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_4[LCD_MATRIX_SIZE_COL + 1];

	Lcd__newLine(lcdLine_1);
	Lcd__newLine(lcdLine_2);
	Lcd__newLine(lcdLine_3);
	Lcd__newLine(lcdLine_4);

	/* line 1 */
	CLock__getDateString(&lcdLine_1[0]);
	CLock__getTimeString(&lcdLine_1[12]);

	/* line 2 */
	strcpy(lcdLine_2, "Temp act:      22.06");

	/* line 4 */
	strcpy(&lcdLine_4[0], "  <SETUP>  <START>  ");

	Lcd__writeLine(lcdLine_1, 1);
	Lcd__writeLine(lcdLine_2, 2);
	Lcd__writeLine(lcdLine_3, 3);
	Lcd__writeLine(lcdLine_4, 4);

	switch (selectState)
	{
		case SEL_SETUP:
		{
			Lcd__setCursor(4, 4);

			if (	(Buttons__isPressedOnce(&buttonFunc1))
				||	(Buttons__isPressedOnce(&buttonFunc2))
			)
			{
				selectState = SEL_START;
			}

			break;
		}

		case SEL_START:
		{
			Lcd__setCursor(4, 13);

			if (	(Buttons__isPressedOnce(&buttonFunc2))
				||	(Buttons__isPressedOnce(&buttonFunc1))
			)
			{
				selectState = SEL_SETUP;
			}

			break;
		}
	}

}


uint8_t Modes__standbyToSetup (void)
{
	return ((selectState == SEL_SETUP) && (Buttons__isPressedOnce(&buttonMode)));
}


uint8_t Modes__standbyToMeasurementStart (void)
{
	return ((selectState == SEL_START) && (Buttons__isPressedOnce(&buttonMode)));
}
