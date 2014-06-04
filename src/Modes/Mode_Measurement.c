/*
 * Mode_Measurement.c
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Measurement.h"
#include "Mode_Setup_Measurement.h"


#define SEL_STOP		1
#define SEL_PAUSE		2
#define	SEL_STATS		3

static uint8_t trigger;
static uint8_t monthStart;
static uint8_t dateStart;
static uint8_t hourStart;
static uint8_t minuteStart;
static uint8_t secondStart;
static uint8_t timeCount;
static uint8_t timePrev;
static uint8_t currentSelectedState;
static uint8_t previousSelectedState;
static uint8_t refresh;
static uint8_t mesPause;
uint8_t monthStart_EEPROM EEMEM;
uint8_t dateStart_EEPROM EEMEM;
uint8_t hourStart_EEPROM EEMEM;
uint8_t minuteStart_EEPROM EEMEM;
uint8_t secondStart_EEPROM EEMEM;
uint8_t timeCount_EEPROM EEMEM;
uint8_t mesPause_EEPROM EEMEM;


void Mode_Measurement__eepromInit (void)
{
	DataLogger__eepromInit();

	monthStart = eeprom_read_byte(&monthStart_EEPROM);
	dateStart = eeprom_read_byte(&dateStart_EEPROM);
	hourStart = eeprom_read_byte(&hourStart_EEPROM);
	minuteStart = eeprom_read_byte(&minuteStart_EEPROM);
	secondStart = eeprom_read_byte(&secondStart_EEPROM);
	timeCount = eeprom_read_byte(&timeCount_EEPROM);
	mesPause = eeprom_read_byte(&mesPause_EEPROM);

	if (mesPause == 0xFF)
	{
		mesPause = 0;
	}
}


static void Mode_Measurement__eepromStorage (void)
{
	eeprom_update_byte(&monthStart_EEPROM, monthStart);
	eeprom_update_byte(&dateStart_EEPROM, dateStart);
	eeprom_update_byte(&hourStart_EEPROM, hourStart);
	eeprom_update_byte(&minuteStart_EEPROM, minuteStart);
	eeprom_update_byte(&secondStart_EEPROM, secondStart);
	eeprom_update_byte(&timeCount_EEPROM, timeCount);
	eeprom_update_byte(&mesPause_EEPROM, mesPause);
}


void Mode_Measurement__init (void)
{
	DataLogger__startMeasure(&Temperature__getCurrentRawValue, &Mode_Measurement__getTrigger);

	monthStart = Clock__getMonth();
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

	Lcd__enableCursor();
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

	trigger = TRUE;
	refresh = TRUE;
}


static void Mode_Measurement__checkTriggers (void)
{
	if (mesPause == FALSE)
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
	uint8_t lastMesDate = 0, lastMesMonth = 0, lastMesHour = 0, lastMesMin = 0, lastMesSec = 0;

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
	if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_DAY)
	{
		lastMesMonth = (monthStart + (dateStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) / 30) % 12;
		lastMesDate = (dateStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) % 30;
		lastMesHour = hourStart;
		lastMesMin = minuteStart;
		lastMesSec = secondStart;
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_HOUR)
	{
		lastMesMonth = (monthStart + (secondStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) / 2592000) % 60;
		lastMesDate = (dateStart + (hourStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) / 24) % 30;
		lastMesHour = (hourStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) % 24;
		lastMesMin = minuteStart;
		lastMesSec = secondStart;
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_MINUTE)
	{
		lastMesMonth = (monthStart + (minuteStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) / 43200) % 60;
		lastMesDate = (dateStart + (minuteStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) / 1440) % 30;
		lastMesHour = (hourStart + (minuteStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) / 60) % 24;
		lastMesMin = (minuteStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) % 60;
		lastMesSec = secondStart;
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_SECOND)
	{
		lastMesMonth = (monthStart + (secondStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) / 2592000) % 60;
		lastMesDate = (dateStart + (secondStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) / 86400) % 30;
		lastMesHour = (hourStart + (secondStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) / 3600) % 24;
		lastMesMin = (minuteStart + (secondStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) / 60) % 60;
		lastMesSec = (secondStart + ((DataLogger__getNumberOfStoredValues() - 1) * Mode_SetupMeasurement__getInterval())) % 60;
	}

	CLock__convertDateToString(lastMesDate, lastMesMonth, &lcdLine_3[0]);
	CLock__convertTimeWithSecondsToString(lastMesHour, lastMesMin, lastMesSec, &lcdLine_3[6]);
	Temperature__getValueStringFromRaw(DataLogger__getStoredValue(DataLogger__getNumberOfStoredValues()), &lcdLine_3[14]);

	if (mesPause == FALSE)
	{
		/* line 4 */
		strcpy(&lcdLine_4[0], "<STOP> <PAUS> <STAT>");
	}
	else
	{
		/* line 4 */
		strcpy(&lcdLine_4[0], "       <REPRENDRE>  ");
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

	if (mesPause == FALSE)
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
				else if (Buttons__isPressedOnce(&buttonMode))
				{
					DataLogger__stopMeasure();
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
					mesPause = TRUE;
					refresh = TRUE;
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
			mesPause = FALSE;
			Mode_Measurement__resume();
		}
	}

	Mode_Measurement__eepromStorage();
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
