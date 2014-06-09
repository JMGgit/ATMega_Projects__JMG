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

#define MEASUREMENT_NB	10

typedef struct
{
	uint8_t monthStart;
	uint8_t dateStart;
	uint8_t hourStart;
	uint8_t minuteStart;
	uint8_t secondStart;
} measurementInfos_t;

static measurementInfos_t measurementInfos[MEASUREMENT_NB];
static uint8_t trigger;
static uint8_t timeCount;
static uint8_t timePrev;
static uint8_t currentSelectedState;
static uint8_t previousSelectedState;
static uint8_t refresh;
static uint8_t currentMesNb;
measurementInfos_t measurementInfos_EEPROM[MEASUREMENT_NB] EEMEM;
uint8_t currentMesNb_EEPROM EEMEM;


void Mode_Measurement__eepromInit (void)
{
	DataLogger__eepromInit();
	eeprom_read_block(measurementInfos, &measurementInfos_EEPROM, MEASUREMENT_NB * sizeof(measurementInfos_t));
	currentMesNb = eeprom_read_byte(&currentMesNb_EEPROM);

	if (currentMesNb == 0xFF)
	{
		currentMesNb = 0;
	}
}


static void Mode_Measurement__eepromStorage (void)
{
	eeprom_update_block(measurementInfos, &measurementInfos_EEPROM, MEASUREMENT_NB * sizeof(measurementInfos_t));
	eeprom_update_byte(&currentMesNb_EEPROM, currentMesNb);
}


void Mode_Measurement__init (void)
{
	DataLogger__startMeasure(&Temperature__getCurrentRawValue, &Mode_Measurement__getTrigger);

	measurementInfos[currentMesNb].monthStart = Clock__getMonth();
	measurementInfos[currentMesNb].dateStart = Clock__getDate();
	measurementInfos[currentMesNb].hourStart = Clock__getHours();
	measurementInfos[currentMesNb].minuteStart = Clock__getMinutes();
	measurementInfos[currentMesNb].secondStart = Clock__getSeconds();

	timeCount = Mode_SetupMeasurement__getInterval();
	currentMesNb++;

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


static void Mode_Measurement__checkTriggers (void)
{
	if (timeCount == Mode_SetupMeasurement__getInterval())
	{
		trigger = TRUE;
		timeCount = 0;
	}
	else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_DAY) && (Clock__getDate() != timePrev)
			&& 	(Clock__getHours() == measurementInfos[currentMesNb - 1].hourStart)
			&&	(Clock__getMinutes() == measurementInfos[currentMesNb - 1].minuteStart)
			&& 	(Clock__getSeconds() == measurementInfos[currentMesNb - 1].secondStart)
	)
	{
		timePrev = Clock__getDate();
		timeCount++;
	}
	else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_HOUR) && (Clock__getHours() != timePrev)
			&&	(Clock__getMinutes() == measurementInfos[currentMesNb - 1].minuteStart)
			&& 	(Clock__getSeconds() == measurementInfos[currentMesNb - 1].secondStart)
	)
	{
		timePrev = Clock__getHours();
		timeCount++;
	}
	else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_MINUTE) && (Clock__getMinutes() != timePrev)
			&& 	(Clock__getSeconds() == measurementInfos[currentMesNb - 1].secondStart)
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
		lastMesMonth = (measurementInfos[currentMesNb - 1].monthStart + (measurementInfos[currentMesNb - 1].dateStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) / 30) % 12;
		lastMesDate = (measurementInfos[currentMesNb - 1].dateStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) % 30;
		lastMesHour = measurementInfos[currentMesNb - 1].hourStart;
		lastMesMin = measurementInfos[currentMesNb - 1].minuteStart;
		lastMesSec = measurementInfos[currentMesNb - 1].secondStart;
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_HOUR)
	{
		lastMesMonth = (measurementInfos[currentMesNb - 1].monthStart + (measurementInfos[currentMesNb - 1].secondStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) / 2592000) % 60;
		lastMesDate = (measurementInfos[currentMesNb - 1].dateStart + (measurementInfos[currentMesNb - 1].hourStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) / 24) % 30;
		lastMesHour = (measurementInfos[currentMesNb - 1].hourStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) % 24;
		lastMesMin = measurementInfos[currentMesNb - 1].minuteStart;
		lastMesSec = measurementInfos[currentMesNb - 1].secondStart;
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_MINUTE)
	{
		lastMesMonth = (measurementInfos[currentMesNb - 1].monthStart + (measurementInfos[currentMesNb - 1].minuteStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) / 43200) % 60;
		lastMesDate = (measurementInfos[currentMesNb - 1].dateStart + (measurementInfos[currentMesNb - 1].minuteStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) / 1440) % 30;
		lastMesHour = (measurementInfos[currentMesNb - 1].hourStart + (measurementInfos[currentMesNb - 1].minuteStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) / 60) % 24;
		lastMesMin = (measurementInfos[currentMesNb - 1].minuteStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) % 60;
		lastMesSec = measurementInfos[currentMesNb - 1].secondStart;
	}
	else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_SECOND)
	{
		lastMesMonth = (measurementInfos[currentMesNb - 1].monthStart + (measurementInfos[currentMesNb - 1].secondStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) / 2592000) % 60;
		lastMesDate = (measurementInfos[currentMesNb - 1].dateStart + (measurementInfos[currentMesNb - 1].secondStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) / 86400) % 30;
		lastMesHour = (measurementInfos[currentMesNb - 1].hourStart + (measurementInfos[currentMesNb - 1].secondStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) / 3600) % 24;
		lastMesMin = (measurementInfos[currentMesNb - 1].minuteStart + (measurementInfos[currentMesNb - 1].secondStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) / 60) % 60;
		lastMesSec = (measurementInfos[currentMesNb - 1].secondStart + ((DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1) - 1) * Mode_SetupMeasurement__getInterval())) % 60;
	}

	CLock__convertDateToString(lastMesDate, lastMesMonth, &lcdLine_3[0]);
	CLock__convertTimeWithSecondsToString(lastMesHour, lastMesMin, lastMesSec, &lcdLine_3[6]);
	Temperature__getValueStringFromRaw(DataLogger__getStoredValueOfMeasure(currentMesNb - 1, DataLogger__getNumberOfStoredValuesOfMeasure(currentMesNb - 1)), &lcdLine_3[14]);

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
