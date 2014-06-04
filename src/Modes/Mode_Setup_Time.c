/*
 * Mode_Setup_Time.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#include "Mode_Setup_Time.h"
#include "Modes.h"

#define SEL_DAY				1
#define SEL_MONTH			2
#define SEL_YEAR			3
#define SEL_HOUR			4
#define SEL_MIN				5
#define SEL_BACK			6

static uint8_t currentSelectedState, previousSelectedState, refresh;


void Mode_SetupTime__init (void)
{
	Lcd__enableCursor();
	currentSelectedState = SEL_DAY;
	previousSelectedState = 0xFF;
	refresh = TRUE;
}


void Mode_SetupTime__x10 (void)
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
	strcpy(&lcdLine_1[0], "Jour Mois An   He Mi");

	/* line 2 */
	CLock__getDateString(&lcdLine_2[0]);
	strcpy(&lcdLine_2[3], "- ");
	CLock__getMonthString(&lcdLine_2[5]);
	strcpy(&lcdLine_2[8], "- ");
	CLock__getYearString(&lcdLine_2[10]);
	CLock__getHourString(&lcdLine_2[15]);
	strcpy(&lcdLine_2[17], ":");
	CLock__getMinutesString(&lcdLine_2[18]);

	/* line 4 */
	strcpy(&lcdLine_4[0], " <RETOUR>           ");

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
		case SEL_DAY:
		{
			Lcd__setCursor(2, 1);

			if (Buttons__isPressedOnce(&buttonMode))
			{
				Clock__incDate();
				refresh = TRUE;
			}

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_MONTH;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_BACK;
			}

			break;
		}

		case SEL_MONTH:
		{
			Lcd__setCursor(2, 6);

			if (Buttons__isPressedOnce(&buttonMode))
			{
				Clock__incMonth();
				refresh = TRUE;
			}

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_YEAR;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_DAY;
			}

			break;
		}

		case SEL_YEAR:
		{
			Lcd__setCursor(2, 11);

			if (Buttons__isPressedOnce(&buttonMode))
			{
				Clock__incYear();
				refresh = TRUE;
			}

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_HOUR;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_MONTH;
			}

			break;
		}

		case SEL_HOUR:
		{
			Lcd__setCursor(2, 16);

			if (Buttons__isPressedOnce(&buttonMode))
			{
				Clock__incHours();
				refresh = TRUE;
			}

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_MIN;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_YEAR;
			}

			break;
		}

		case SEL_MIN:
		{
			Lcd__setCursor(2, 19);

			if (Buttons__isPressedOnce(&buttonMode))
			{
				Clock__incMinutes();
				refresh = TRUE;
			}

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_BACK;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_HOUR;
			}

			break;
		}

		case SEL_BACK:
		{
			Lcd__setCursor(4, 3);

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_DAY;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_MIN;
			}

			break;
		}
	}

	if (previousSelectedState != currentSelectedState)
	{
		refresh = TRUE;
		previousSelectedState = currentSelectedState;
	}
}


uint8_t Modes__setupTimeToSetup (void)
{
	return ((currentSelectedState == SEL_BACK) && (Buttons__isPressedOnce(&buttonMode)));
}

