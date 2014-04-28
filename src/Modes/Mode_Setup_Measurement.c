/*
 * Mode_Setup_Measurement.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#include "Mode_Setup_Measurement.h"

#define SEL_INTERVAL	0
#define	SEL_UNIT		1
#define SEL_BACK		2

#define UNIT_SECOND		0
#define UNIT_MINUTE	1
#define UNIT_HOUR		2
#define UNIT_DAY		3

static uint8_t currentSelectedState, previousSelectedState, refresh, unit;
static uint16_t interval;
static uint16_t interval_EEPROM EEMEM = 0;
static uint8_t unit_EEPROM EEMEM = 0;


void Mode_SetupMeasurement__init (void)
{
	Lcd__enableCursor();
	currentSelectedState = SEL_INTERVAL;
	previousSelectedState = 0xFF;
	interval = eeprom_read_word(&interval_EEPROM);
	unit = eeprom_read_byte(&unit_EEPROM);
}


static void Mode_SetupMeasurement__getIntervalString (char* buffer)
{
	uint8_t idxBuffer = 0;

	itoa(interval, &buffer[idxBuffer++], 10);
	if (interval >= 10)
	{
		idxBuffer++;
	}

	buffer[idxBuffer] = ' ';
}


static void Mode_SetupMeasurement__getUnitString (char* buffer)
{
	if (unit == UNIT_SECOND)
	{
		strcpy(&buffer[0], "secs ");
	}
	else if (unit == UNIT_MINUTE)
	{
		strcpy(&buffer[0], "mins ");
	}
	else if (unit == UNIT_HOUR)
	{
		strcpy(&buffer[0], "heure");
	}
	else if (unit == UNIT_DAY)
	{
		strcpy(&buffer[0], "jours");
	}

	buffer[5] = ' ';
}


static void Mode_SetupMeasurement__getMeasurementTimeString (char* buffer)
{
	uint16_t days, hours, minutes;
	uint8_t idxBuffer = 0;

	if (unit == UNIT_SECOND)
	{
		days = (MAX_MEASUREMENT_POINTS * interval) / 86400;
		hours = (MAX_MEASUREMENT_POINTS * interval)/3600; // (MAX_MEASUREMENT_POINTS * interval) / 3600;// - (days * 86400);
		minutes = ((MAX_MEASUREMENT_POINTS * interval) - (days * 86400) - (hours * 3600)) / 60;
	}
	else if (unit == UNIT_MINUTE)
	{
		days = (MAX_MEASUREMENT_POINTS * interval) / 1440;
		hours = (MAX_MEASUREMENT_POINTS * interval) / 60 - (days * 24);
		minutes = (MAX_MEASUREMENT_POINTS * interval) - (days * 1440) - (hours * 60);
	}
	else if (unit == UNIT_HOUR)
	{
		days = (MAX_MEASUREMENT_POINTS * interval) / 24;
		hours = (MAX_MEASUREMENT_POINTS * interval) - (days * 24);
		minutes = 0;
	}
	else if (unit == UNIT_DAY)
	{
		days = MAX_MEASUREMENT_POINTS * interval;
		hours = 0;
		minutes = 0;
	}

	if (days > 9999)
	{
		days = 9999;
		hours = 99;
		minutes = 99;
	}

	itoa(days, &buffer[idxBuffer++], 10);

	if (days >= 10)
	{
		idxBuffer++;
	}

	if (days >= 100)
	{
		idxBuffer++;
	}

	if (days >= 1000)
	{
		idxBuffer++;
	}

	strcpy(&buffer[idxBuffer++], "j");
	strcpy(&buffer[idxBuffer++], " ");

	if (hours >= 10)
	{
		itoa(hours, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(hours, &buffer[idxBuffer++], 10);
	}

	strcpy(&buffer[idxBuffer++], "h");
	strcpy(&buffer[idxBuffer++], " ");

	if (minutes >= 10)
	{
		itoa(minutes, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(minutes, &buffer[idxBuffer++], 10);
	}

	strcpy(&buffer[idxBuffer++], "m");

	buffer[idxBuffer++] = ' ';

}


static void Mode_SetupMeasurement__eepromStorage (void)
{
	eeprom_update_word(&interval_EEPROM, interval);
	eeprom_update_byte(&unit_EEPROM, unit);
}


void Mode_SetupMeasurement__x10 (void)
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
		case SEL_INTERVAL:
		{
			Lcd__setCursor(1, 13);

			if (Buttons__isPressedOnce(&buttonMode))
			{
				if (interval < 20)
				{
					interval++;
				}
				else
				{
					interval = 1;
				}

				refresh = TRUE;
			}

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_UNIT;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_BACK;
			}

			break;
		}

		case SEL_UNIT:
		{
			Lcd__setCursor(1, 16);

			if (Buttons__isPressedOnce(&buttonMode))
			{
				if (unit == UNIT_SECOND)
				{
					unit = UNIT_MINUTE;
				}
				else if (unit == UNIT_MINUTE)
				{
					unit = UNIT_HOUR;
				}
				else if (unit == UNIT_HOUR)
				{
					unit = UNIT_DAY;
				}
				else
				{
					unit = UNIT_SECOND;
				}

				refresh = TRUE;
			}

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_BACK;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_INTERVAL;
			}

			break;
		}

		case SEL_BACK:
		{
			Lcd__setCursor(4, 3);

			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				currentSelectedState = SEL_INTERVAL;
			}

			if (Buttons__isPressedOnce(&buttonFunc1))
			{
				currentSelectedState = SEL_UNIT;
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

uint8_t Modes__setupMeasurementToSetup (void)
{
	uint8_t retVal = FALSE;

	if ((currentSelectedState == SEL_BACK) && (Buttons__isPressedOnce(&buttonMode)))
	{
		Mode_SetupMeasurement__eepromStorage();
		retVal = TRUE;
	}

	return retVal;
}

