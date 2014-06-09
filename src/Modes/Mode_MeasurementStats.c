/*
 * Mode_MeasurementStats.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */


#include "Mode_MeasurementStats.h"
#include "Mode_Measurement.h"


#define SEL_BACK		1
#define SEL_NEXT		2
#define SEL_PREV		3


static uint8_t currentSelectedState;
static uint8_t previousSelectedState;
static uint8_t selectedMeasure;
static uint8_t refresh;


void Mode_MeasurementStats__init (void)
{

}


void Mode_MeasurementStats__x10 (void)
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
	strcpy(lcdLine_1, "<1>                 ");
	DataLogger__getStartTimeOfMeasure (1, &month, &date, &hour, &min, &sec);

	if ((month != 0) && (date != 0))
	{
		CLock__convertDateToString(date, month, &lcdLine_1[4]);
		CLock__convertTimeWithSecondsToString(hour, min, sec, &lcdLine_1[10]);
	}

	/* line 2 */
	strcpy(lcdLine_2, "<2>                 ");

	DataLogger__getStartTimeOfMeasure (2, &month, &date, &hour, &min, &sec);

	if ((month != 0) && (date != 0))
	{
		CLock__convertDateToString(date, month, &lcdLine_2[4]);
		CLock__convertTimeWithSecondsToString(hour, min, sec, &lcdLine_2[10]);
	}

	/* line 3 */
	strcpy(lcdLine_3, "<3>                 ");
	DataLogger__getStartTimeOfMeasure (3, &month, &date, &hour, &min, &sec);

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

	}

	if (previousSelectedState != currentSelectedState)
	{
		refresh = TRUE;
		previousSelectedState = currentSelectedState;
	}
}


uint8_t Modes__measurementStatsToMeasurement (void)
{
	return FALSE;
}
