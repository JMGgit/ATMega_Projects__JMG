/*
 * Mode_MeasurementStats.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */


#include "Mode_MeasurementStats.h"
#include "Mode_Measurement.h"


#define SEL_STOP		1
#define	SEL_STATS		2


static uint8_t currentSelectedState;
static uint8_t previousSelectedState;
static uint8_t refresh;


void Mode_MeasurementStats__init (void)
{
	Mode_Measurement__init();
}


void Mode_MeasurementStats__x10 (void)
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
	//strcpy(lcdLine_1, "Temp act:     ");
	//Temperature__getCurrentValueString(&lcdLine_1[14]);

	/* line 2 */
	//strcpy(lcdLine_2, "Mesure precedente:   ");

	/* line 3 */

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


uint8_t Modes__measurementStatsToMeasurement (void)
{
	return FALSE;
}
