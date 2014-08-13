/*
 * Mode_MeasurementStart.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */


#include "Mode_MeasurementStart.h"
#include "Mode_Setup_Measurement.h"


#define SEL_BACK			1
#define SEL_START			2

static uint8_t currentSelectedState, previousSelectedState, refresh;



void Mode_MeasurementStart__init (void)
{
	Lcd__enableCursor();
	currentSelectedState = SEL_START;
	previousSelectedState = 0xFF;
}


void Mode_MeasurementStart__x10 (void)
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
	strcpy(&lcdLine_1[0], "Intervalle: ");
	Mode_SetupMeasurement__getIntervalString(&lcdLine_1[12]);
	Mode_SetupMeasurement__getUnitString(&lcdLine_1[15]);

	/* line 2 */
	strcpy(&lcdLine_2[0], "Max: ");
	Mode_SetupMeasurement__getMeasurementTimeString(&lcdLine_2[5]);

	/* line 3 */
	if (DataLogger__getNumberOfMeasures() >= DATA_LOGGER_MEASURES_NB)
	{
		strcpy(&lcdLine_3[0], "!! MEMOIRE PLEINE !!");
	}

	else
	{
		strcpy(&lcdLine_3[0], "Mesures possibles:  ");
		itoa(DATA_LOGGER_MEASURES_NB - DataLogger__getNumberOfMeasures(), &lcdLine_3[19], 10);
	}
	/* line 4 */
	strcpy(&lcdLine_4[0], " <RETOUR>  <START>  ");

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
			Lcd__setCursor(4, 3);

			if (	(Buttons__isPressedOnce(&buttonFunc1))
				||	(Buttons__isPressedOnce(&buttonFunc2))
			)
			{
				currentSelectedState = SEL_START;
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


uint8_t Modes__measurementStartToMeasurement (void)
{
	return (		(currentSelectedState == SEL_START) && (Buttons__isPressedOnce(&buttonMode))
				&&	(DataLogger__getNumberOfMeasures() < DATA_LOGGER_MEASURES_NB)
				&&	(DataLogger__getFreeMeasurementPoints() > 0)
	);
}


uint8_t Modes__measurementStartToStandby (void)
{
	return ((currentSelectedState == SEL_BACK) && (Buttons__isPressedOnce(&buttonMode)));
}
