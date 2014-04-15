/*
 * Mode_Setup.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Setup.h"

#define SEL_CLOCK			0
#define	SEL_MEASUREMENT		1
#define SEL_BACK			2

static uint8_t selectState = SEL_CLOCK;


void Mode_Setup__init (void)
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
	strcpy(&lcdLine_1[0], "------ SETUP -------");

	/* line 2 */
	strcpy(&lcdLine_2[0], " <Date/heure>       ");

	/* line 3 */
	strcpy(&lcdLine_3[0], " <Mesures>          ");

	/* line 4 */
	strcpy(&lcdLine_4[0], " <RETOUR>           ");

	Lcd__writeLine(lcdLine_1, 1);
	Lcd__writeLine(lcdLine_2, 2);
	Lcd__writeLine(lcdLine_3, 3);
	Lcd__writeLine(lcdLine_4, 4);

	Lcd__enableCursor();
	selectState = SEL_CLOCK;
}


void Mode_Setup__x10 (void)
{
	switch (selectState)
	{
		case SEL_CLOCK:
		{
			Lcd__setCursor(2, 3);

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				selectState = SEL_MEASUREMENT;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				selectState = SEL_BACK;
			}

			break;
		}

		case SEL_MEASUREMENT:
		{
			Lcd__setCursor(3, 3);

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				selectState = SEL_BACK;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				selectState = SEL_CLOCK;
			}

			break;
		}

		case SEL_BACK:
		{
			Lcd__setCursor(4, 3);

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				selectState = SEL_CLOCK;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				selectState = SEL_MEASUREMENT;
			}

			break;
		}
	}
}


uint8_t Modes__setupToStandby (void)
{
	return ((selectState == SEL_BACK) && (Buttons__isPressedOnce(&buttonMode)));
}
