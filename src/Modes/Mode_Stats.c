/*
 * Mode_Stats.c
 *
 *  Created on: 09.06.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Stats.h"


#define SEL_BACK		1
#define SEL_NEXT		2
#define SEL_PREV		3
#define SEL_MEASURE_1	4
#define SEL_MEASURE_2	5
#define SEL_MEASURE_3	6

#define STATS_SCREENS_NB (DATA_LOGGER_MEASURES_NB / 3)

static uint8_t currentSelectedState;
static uint8_t previousSelectedState;
static uint8_t screen;
static uint8_t refresh;


void Mode_Stats__init (void)
{
	Lcd__enableCursor();

	if (DataLogger__getNumberOfMeasures() != 0)
	{
		currentSelectedState = SEL_MEASURE_1;
	}
	else
	{
		currentSelectedState = SEL_BACK;
	}

	screen = 0;
}


void Mode_Stats__x10 (void)
{
	uint8_t month, date, hour, min, sec;
	char lcdLine_1[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_2[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_3[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_4[LCD_MATRIX_SIZE_COL + 1];

	Lcd__newLine(lcdLine_1);
	Lcd__newLine(lcdLine_2);
	Lcd__newLine(lcdLine_3);
	Lcd__newLine(lcdLine_4);

	/* line 1 */
	strcpy(lcdLine_1, "<");
	itoa((screen * 3) + 1, &lcdLine_1[1], 10);
	strcpy(&lcdLine_1[2], ">                 ");

	DataLogger__getStartTimeOfMeasure ((screen * 3) + 1, &month, &date, &hour, &min, &sec);

	if ((month != 0) && (date != 0))
	{
		CLock__convertDateToString(date, month, &lcdLine_1[4]);
		CLock__convertTimeWithSecondsToString(hour, min, sec, &lcdLine_1[10]);
	}

	/* line 2 */
	strcpy(lcdLine_2, "<");
	itoa((screen * 3) + 2, &lcdLine_2[1], 10);
	strcpy(&lcdLine_2[2], ">                 ");

	DataLogger__getStartTimeOfMeasure ((screen * 3) + 2, &month, &date, &hour, &min, &sec);

	if ((month != 0) && (date != 0))
	{
		CLock__convertDateToString(date, month, &lcdLine_2[4]);
		CLock__convertTimeWithSecondsToString(hour, min, sec, &lcdLine_2[10]);
	}

	/* line 3 */
	strcpy(lcdLine_3, "<");
	itoa((screen * 3) + 3, &lcdLine_3[1], 10);
	strcpy(&lcdLine_3[2], ">                 ");
	DataLogger__getStartTimeOfMeasure ((screen * 3) + 3, &month, &date, &hour, &min, &sec);

	if ((month != 0) && (date != 0))
	{
		CLock__convertDateToString(date, month, &lcdLine_3[4]);
		CLock__convertTimeWithSecondsToString(hour, min, sec, &lcdLine_3[10]);
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
				if (DataLogger__getNumberOfMeasures() > ((3 * screen) + 2))
				{
					currentSelectedState = SEL_MEASURE_3;
				}
				else if (DataLogger__getNumberOfMeasures() > ((3 * screen) + 1))
				{
					currentSelectedState = SEL_MEASURE_2;
				}
				else if (DataLogger__getNumberOfMeasures() > ((3 * screen)))
				{
					currentSelectedState = SEL_MEASURE_1;
				}
				else
				{
					currentSelectedState = SEL_NEXT;
				}
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
				if (DataLogger__getNumberOfMeasures() > (3 * screen))
				{
					currentSelectedState = SEL_MEASURE_1;
				}
				else
				{
					currentSelectedState = SEL_BACK;
				}
			}
			else if (	(screen < (STATS_SCREENS_NB - 1))
					&& 	(Buttons__isPressedOnce(&buttonMode))
			)
			{
				screen++;
				refresh = TRUE;
			}

			break;
		}

		case SEL_MEASURE_1:
		{
			Lcd__setCursor(1, 2);

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_NEXT;
			}
			else if (Buttons__isPressedOnce(&buttonFunc2))
			{
				if (DataLogger__getNumberOfMeasures() > ((3 * screen) + 1))
				{
					currentSelectedState = SEL_MEASURE_2;
				}
				else
				{
					currentSelectedState = SEL_BACK;
				}
			}

			break;
		}

		case SEL_MEASURE_2:
		{
			Lcd__setCursor(2, 2);

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_MEASURE_1;
			}
			else if (Buttons__isPressedOnce(&buttonFunc2))
			{
				if (DataLogger__getNumberOfMeasures() > ((3 * screen) + 2))
				{
					currentSelectedState = SEL_MEASURE_3;
				}
				else
				{
					currentSelectedState = SEL_BACK;
				}
			}

			break;
		}

		case SEL_MEASURE_3:
		{
			Lcd__setCursor(3, 2);

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_MEASURE_2;
			}
			else if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_BACK;
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


uint8_t Modes__statsToStandby (void)
{
	return ((currentSelectedState == SEL_BACK) && (Buttons__isPressedOnce(&buttonMode)));
}


uint8_t Modes__statsToStatsMeasure (uint8_t *measure)
{
	uint8_t retVal;

	if (Buttons__isPressedOnce(&buttonMode))
	{
		if (currentSelectedState == SEL_MEASURE_1)
		{
			*measure = (3 * screen) + 1;
			retVal = TRUE;
		}
		else if (currentSelectedState == SEL_MEASURE_2)
		{
			*measure = (3 * screen) + 2;
			retVal = TRUE;
		}
		else if (currentSelectedState == SEL_MEASURE_3)
		{
			*measure = (3 * screen) + 3;
			retVal = TRUE;
		}
		else
		{
			retVal = FALSE;
		}
	}
	else
	{
		retVal = FALSE;
	}

	return retVal;
}
