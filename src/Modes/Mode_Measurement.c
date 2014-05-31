/*
 * Mode_Measurement.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Measurement.h"
#include "Mode_Setup_Measurement.h"


#define SEL_STOP		0
#define	SEL_STATS		1

static uint8_t trigger;
static uint8_t dateCount;
static uint8_t hourCount;
static uint8_t minuteCount;
static uint8_t secondCount;
static uint8_t currentSelectedState, previousSelectedState, refresh;

void Mode_Measurement__eepromInit (void)
{
	DataLogger__eepromInit();
}

void Mode_Measurement__init (void)
{
	DataLogger__startMeasure(&Temperature__getCurrentRawValue, &Mode_Measurement__getTrigger);
	dateCount = Clock__getDate();
	hourCount = Clock__getHours();
	minuteCount = Clock__getMinutes();
	secondCount = Clock__getSeconds();
	trigger = TRUE;
}


static void Mode_Measurement__checkTriggers (void)
{
	if (		(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_DAY)
			&&	(Clock__getDate() != dateCount)
			&& 	(Clock__getHours() == hourCount)
			&&	(Clock__getMinutes() == minuteCount)
			&& (Clock__getSeconds() == secondCount)
			&&	(		((Clock__getDate() > dateCount) &&  (Clock__getDate() == (dateCount + Mode_SetupMeasurement__getInterval())))
					||	((Clock__getDate() < dateCount) &&  (((Clock__getDate() + 60) == (dateCount + Mode_SetupMeasurement__getInterval()))))
			)
	)
	{
		trigger = TRUE;
		dateCount = Clock__getDate();
	}

	if (		(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_HOUR)
			&&	(Clock__getHours() != hourCount)
			&&	(Clock__getMinutes() == minuteCount)
			&&	(Clock__getSeconds() == secondCount)
			&&	(		((Clock__getHours() > hourCount) &&  (Clock__getHours() == (hourCount + Mode_SetupMeasurement__getInterval())))
					||	((Clock__getHours() < hourCount) &&  (((Clock__getHours() + 60) == (hourCount + Mode_SetupMeasurement__getInterval()))))
			)
	)
	{
		trigger = TRUE;
		hourCount = Clock__getHours();
	}

	if (		(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_MINUTE)
			&&	(Clock__getMinutes() != minuteCount)
			&&	(Clock__getSeconds() == secondCount)
			&&	(		((Clock__getMinutes() > minuteCount) &&  (Clock__getMinutes() == (minuteCount + Mode_SetupMeasurement__getInterval())))
					||	((Clock__getMinutes() < minuteCount) &&  (((Clock__getMinutes() + 60) == (minuteCount + Mode_SetupMeasurement__getInterval()))))
			)
	)
	{
		trigger = TRUE;
		minuteCount = Clock__getMinutes();
	}

	if (		(Clock__getSeconds() != secondCount)
			&&	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_SECOND)
			&&	(		((Clock__getSeconds() > secondCount) &&  (Clock__getSeconds() == (secondCount + Mode_SetupMeasurement__getInterval())))
					||	((Clock__getSeconds() < secondCount) &&  (((Clock__getSeconds() + 60) == (secondCount + Mode_SetupMeasurement__getInterval()))))
			)
	)
	{
		trigger = TRUE;
		secondCount = Clock__getSeconds();
	}

}

void Mode_Measurement__x10 (void)
{
	Mode_Measurement__checkTriggers();
	DataLogger__x10();

	char lcdLine_1[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_2[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_3[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_4[LCD_MATRIX_SIZE_COL + 1];

	Lcd__newLine(lcdLine_1);
	Lcd__newLine(lcdLine_2);
	Lcd__newLine(lcdLine_3);
	Lcd__newLine(lcdLine_4);

	/* line 1 */
	strcpy(lcdLine_1, "Temp act:     ");
	Temperature__getCurrentValueString(&lcdLine_1[14]);

	/* line 2 */
	strcpy(lcdLine_2, "Mesure precedente:   ");

	/* line 3 */
	CLock__getCompleteDateString(&lcdLine_3[0]);
	CLock__getTimeWithSecondsString(&lcdLine_3[6]);
	Temperature__getValueStringFromRaw(DataLogger__getStoredValue(DataLogger__getNumberOfStoredValues() - 1), &lcdLine_3[14]);

	/* line 4 */
	strcpy(&lcdLine_4[0], " <STOP>     <STATS> ");

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
		case SEL_STOP:
		{
			Lcd__setCursor(4, 3);

			if (	(Buttons__isPressedOnce(&buttonFunc1))
				||	(Buttons__isPressedOnce(&buttonFunc2))
			)
			{
				currentSelectedState = SEL_STATS;
			}

			break;
		}

		case SEL_STATS:
		{
			Lcd__setCursor(4, 14);

			if (	(Buttons__isPressedOnce(&buttonFunc2))
				||	(Buttons__isPressedOnce(&buttonFunc1))
			)
			{
				currentSelectedState = SEL_STOP;
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


uint8_t Mode_Measurement__getTrigger (void)
{
	if (trigger == TRUE)
	{
		trigger = FALSE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



uint8_t Modes__measurementToMeasurementStats (void)
{
	return ((currentSelectedState == SEL_STATS) && (Buttons__isPressedOnce(&buttonMode)));
}


uint8_t Modes__measurementToStandby (void)
{
	return ((currentSelectedState == SEL_STOP) && (Buttons__isPressedOnce(&buttonMode)));
}
