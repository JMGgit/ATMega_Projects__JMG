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
}


void Mode_Measurement__x10 (void)
{
	uint8_t lastMesDate = 0, lastMesMonth = 0, lastMesHour = 0, lastMesMin = 0, lastMesSec = 0;
	uint16_t lastTempValue;

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
	strcpy(lcdLine_2, "Derniere mesure:     ");

	/* line 3 */
	DataLogger__getLastValueWithTime(&lastMesMonth, &lastMesDate, &lastMesHour, &lastMesMin, &lastMesSec, &lastTempValue);
	CLock__convertDateToString(lastMesDate, lastMesMonth, &lcdLine_3[0]);
	CLock__convertTimeWithSecondsToString(lastMesHour, lastMesMin, lastMesSec, &lcdLine_3[6]);
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
