/*
 * Mode_Stats_Measure.c
 *
 *  Created on: 10.06.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Stats_Measure.h"


#define SEL_BACK		1
#define SEL_NEXT		2
#define SEL_PREV		3


static uint8_t currentSelectedState;
static uint8_t previousSelectedState;
static uint8_t refresh;
static uint16_t screen;
static uint8_t measureNumber;


void Mode_StatsMeasure__init (uint8_t measure)
{
	Lcd__enableCursor();
	currentSelectedState = SEL_BACK;
	screen = 0;
	measureNumber = measure;
}


void Mode_StatsMeasure__x10 (void)
{
	uint8_t month, date, hour, min, sec;
	uint16_t tempValue;
	char lcdLine_1[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_2[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_3[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_4[LCD_MATRIX_SIZE_COL + 1];

	Lcd__newLine(lcdLine_1);
	Lcd__newLine(lcdLine_2);
	Lcd__newLine(lcdLine_3);
	Lcd__newLine(lcdLine_4);

	/* line 1 */
	DataLogger__getValueWithTime(measureNumber, screen * 3, &month, &date, &hour, &min, &sec, &tempValue);

	if ((month != 0) && (date != 0))
	{
		CLock__convertDateToString(date, month, &lcdLine_1[0]);
		CLock__convertTimeWithSecondsToString(hour, min, sec, &lcdLine_1[6]);
		Temperature__getValueStringFromRaw(tempValue, &lcdLine_1[14]);
	}

	/* line 2 */
	DataLogger__getValueWithTime(measureNumber, (screen * 3) + 1, &month, &date, &hour, &min, &sec, &tempValue);

	if ((month != 0) && (date != 0))
	{
		CLock__convertDateToString(date, month, &lcdLine_2[0]);
		CLock__convertTimeWithSecondsToString(hour, min, sec, &lcdLine_2[6]);
		Temperature__getValueStringFromRaw(tempValue, &lcdLine_2[14]);
	}

	/* line 3 */
	DataLogger__getValueWithTime(measureNumber, (screen * 3) + 2, &month, &date, &hour, &min, &sec, &tempValue);

	if ((month != 0) && (date != 0))
	{
		CLock__convertDateToString(date, month, &lcdLine_3[0]);
		CLock__convertTimeWithSecondsToString(hour, min, sec, &lcdLine_3[6]);
		Temperature__getValueStringFromRaw(tempValue, &lcdLine_3[14]);
	}

	/* line 4 */
	strcpy(&lcdLine_4[0], "<RET>  <PREC> <SUIV>");

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
			Lcd__setCursor(4, 2);

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_NEXT;
			}
			else if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_PREV;
			}

			break;
		}

		case SEL_PREV:
		{
			Lcd__setCursor(4, 9);

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_BACK;
			}
			else if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_NEXT;
			}
			else if (	(screen > 0)
					&& 	(Buttons__isPressedOnce(&buttonMode))
			)
			{
				screen--;
				refresh = TRUE;
			}
			break;
		}

		case SEL_NEXT:
		{
			Lcd__setCursor(4, 16);

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_PREV;
			}
			else if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_BACK;
			}
			else if (	(screen < (((DataLogger__getNumberOfStoredValuesOfMeasure(measureNumber)  - 1) / 3)))
					&& 	(Buttons__isPressedOnce(&buttonMode))
			)
			{
				screen++;
				refresh = TRUE;
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


uint8_t Modes__statsMeasureToStats (void)
{
	return ((currentSelectedState == SEL_BACK) && (Buttons__isPressedOnce(&buttonMode)));
}
