/*
 * Mode_Measurement.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Measurement.h"
#include "Mode_Setup_Measurement.h"


#define SEL_STOP		0
#define SEL_PAUSE		1
#define	SEL_STATS		2

static uint8_t trigger, dateStart, hourStart, minuteStart, secondStart, timeCount, timePrev, currentSelectedState, previousSelectedState, refresh, pause;


void Mode_Measurement__eepromInit (void)
{
	DataLogger__eepromInit();
}

void Mode_Measurement__init (void)
{
	DataLogger__startMeasure(&Temperature__getCurrentRawValue, &Mode_Measurement__getTrigger);

	dateStart = Clock__getDate();
	hourStart = Clock__getHours();
	minuteStart = Clock__getMinutes();
	secondStart = Clock__getSeconds();

	timeCount = Mode_SetupMeasurement__getInterval();

	if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_DAY)
	{
		timePrev = Clock__getDate();
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_HOUR)
	{
		timePrev = Clock__getHours();
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_MINUTE)
	{
		timePrev = Clock__getMinutes();
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_SECOND)
	{
		timePrev = Clock__getSeconds();
	}

	currentSelectedState = SEL_STATS;
}


static void Mode_Measurement__resume (void)
{
	DataLogger__resumeMeasure();

	if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_DAY)
	{
		timePrev = Clock__getDate();
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_HOUR)
	{
		timePrev = Clock__getHours();
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_MINUTE)
	{
		timePrev = Clock__getMinutes();
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_SECOND)
	{
		timePrev = Clock__getSeconds();
	}

	currentSelectedState = SEL_STATS;
}


static void Mode_Measurement__checkTriggers (void)
{
	if (pause == FALSE)
	{
		if (timeCount == Mode_SetupMeasurement__getInterval())
		{
			trigger = TRUE;
			timeCount = 0;
		}
		else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_DAY) && (Clock__getDate() != timePrev)
				&& 	(Clock__getHours() == hourStart)
				&&	(Clock__getMinutes() == minuteStart)
				&& 	(Clock__getSeconds() == secondStart)
		)
		{
			timePrev = Clock__getDate();
			timeCount++;
		}
		else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_HOUR) && (Clock__getHours() != timePrev)
				&&	(Clock__getMinutes() == minuteStart)
				&& 	(Clock__getSeconds() == secondStart)
		)
		{
			timePrev = Clock__getHours();
			timeCount++;
		}
		else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_MINUTE) && (Clock__getMinutes() != timePrev)
				&& 	(Clock__getSeconds() == secondStart)
		)
		{
			timePrev = Clock__getMinutes();
			timeCount++;
		}
		else if ((Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_SECOND) && (Clock__getSeconds() != timePrev))
		{
			timePrev = Clock__getSeconds();
			timeCount++;
		}
	}
	else
	{
		trigger = FALSE;
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
	Temperature__getValueStringFromRaw(DataLogger__getStoredValue(DataLogger__getNumberOfStoredValues()), &lcdLine_3[14]);

	if (pause == FALSE)
	{
		/* line 4 */
		strcpy(&lcdLine_4[0], "<STOP> <PAUS> <STAT>");
	}
	else
	{
		/* line 4 */
		strcpy(&lcdLine_4[0], "       <REPRENDRE> ");
	}

	Lcd__writeLine(lcdLine_1, 1);
	Lcd__writeLine(lcdLine_2, 2);
	Lcd__writeLine(lcdLine_3, 3);
	Lcd__writeLine(lcdLine_4, 4);

	if (refresh == TRUE)
	{
		Lcd__requestRefresh();
		refresh = FALSE;
	}

	if (pause == FALSE)
	{
		switch (currentSelectedState)
		{
			case SEL_STOP:
			{
				Lcd__setCursor(4, 2);

				if (Buttons__isPressedOnce(&buttonFunc1))
				{
					currentSelectedState = SEL_STATS;
				}
				else if (Buttons__isPressedOnce(&buttonFunc2))
				{
					currentSelectedState = SEL_PAUSE;
				}

				break;
			}

			case SEL_PAUSE:
			{
				Lcd__setCursor(4, 9);

				if (Buttons__isPressedOnce(&buttonFunc1))
				{
					currentSelectedState = SEL_STOP;
				}
				else if (Buttons__isPressedOnce(&buttonFunc2))
				{
					currentSelectedState = SEL_STATS;
				}
				else if (Buttons__isPressedOnce(&buttonMode))
				{
					pause = TRUE;
					DataLogger__pauseMeasure();
				}

				break;
			}

			case SEL_STATS:
			{
				Lcd__setCursor(4, 16);

				if (Buttons__isPressedOnce(&buttonFunc1))
				{
					currentSelectedState = SEL_PAUSE;
				}
				else if (Buttons__isPressedOnce(&buttonFunc2))
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
	else
	{
		Lcd__setCursor(4, 9);

		if (Buttons__isPressedOnce(&buttonMode))
		{
			pause = FALSE;
			Mode_Measurement__resume();
		}

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
