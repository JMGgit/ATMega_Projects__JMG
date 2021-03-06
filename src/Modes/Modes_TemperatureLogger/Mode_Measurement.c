/*
 * Mode_Measurement.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Measurement.h"
#include "Mode_Setup_Measurement.h"


#define SEL_STOP		1
#define	SEL_STATS		2


static uint8_t currentSelectedState;
static uint8_t previousSelectedState;
static uint8_t refresh;


void Mode_Measurement__eepromInit (void)
{
	DataLogger__eepromInit();
}


void Mode_Measurement__init (void)
{
	DataLogger__startMeasure(&Temperature__getCurrentRawValue);
	Lcd__enableCursor();
	currentSelectedState = SEL_STATS;
	previousSelectedState = 0xFF;
}


void Mode_Measurement__x10 (void)
{
	uint8_t lastMeasMonth = 0, lastMeasDate = 0, lastMeasHour = 0, lastMeasMin = 0, lastMeasSec = 0;
	uint16_t lastMeasYear = 0;
	uint16_t lastTempValue;
	char lcdLine_1[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_2[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_3[LCD_MATRIX_SIZE_COL + 1];
	char lcdLine_4[LCD_MATRIX_SIZE_COL + 1];

	DataLogger__x10();

	Lcd__newLine(lcdLine_1);
	Lcd__newLine(lcdLine_2);
	Lcd__newLine(lcdLine_3);
	Lcd__newLine(lcdLine_4);

	/* line 1 */
	strcpy(lcdLine_1, "Moy/Act: ");

	DataLogger__getAverageValueString(DataLogger__getNumberOfMeasures(), &lcdLine_1[8]);
	Temperature__getCurrentValueString(&lcdLine_1[14]);

	/* line 2 */
	strcpy(lcdLine_2, "Min/Max: ");
	DataLogger__getMinValueString(DataLogger__getNumberOfMeasures(), &lcdLine_2[8]);
	DataLogger__getMaxValueString(DataLogger__getNumberOfMeasures(), &lcdLine_2[14]);

	/* line 3 */
	DataLogger__getLastValueWithTime(&lastMeasYear, &lastMeasMonth, &lastMeasDate, &lastMeasHour, &lastMeasMin, &lastMeasSec, &lastTempValue);
	Clock__convertDateToString(lastMeasDate, lastMeasMonth, &lcdLine_3[0]);
	Clock__convertTimeWithSecondsToString(lastMeasHour, lastMeasMin, lastMeasSec, &lcdLine_3[6]);
	Temperature__getValueStringFromRaw(lastTempValue, &lcdLine_3[14]);

	/* line 4 */
	strcpy(&lcdLine_4[0], "<STOP>       <STATS>");

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
			Lcd__setCursor(4, 2);

			if ((Buttons__isPressedOnce(&buttonFunc1)) || (Buttons__isPressedOnce(&buttonFunc2)))
			{
				currentSelectedState = SEL_STATS;
			}
			else if (Buttons__isPressedOnce(&buttonMode))
			{
				DataLogger__stopMeasure();
			}
			break;
		}

		case SEL_STATS:
		{
			Lcd__setCursor(4, 15);

			if ((Buttons__isPressedOnce(&buttonFunc1)) || (Buttons__isPressedOnce(&buttonFunc2)))
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


uint8_t Modes__measurementToMeasurementStats (void)
{
	return ((currentSelectedState == SEL_STATS) && (Buttons__isPressedOnce(&buttonMode)));
}


uint8_t Modes__measurementToStandby (void)
{
	return ((currentSelectedState == SEL_STOP) && (Buttons__isPressedOnce(&buttonMode)));
}
