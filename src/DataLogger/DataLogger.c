/*
 * DataLogger.c
 *
 *  Created on: 28.05.2014
 *      Author: Jean-Martin George
 */


#include "DataLogger.h"
#include "../Modes/Mode_Measurement.h"


#define DL_MODE__IDLE				1
#define DL_MODE__MEASUREMENT		2
#define DL_MODE__PAUSE				3

static uint16_t dataLog[MAX_MEASUREMENT_POINTS];
uint16_t dataLog_EEPROM[MAX_MEASUREMENT_POINTS] EEMEM;

static uint8_t mode;
static uint16_t (*getCurrentData)();
static uint8_t (*getTrigger)();
static uint16_t dataLogIt;
uint8_t dlMode_EEPROM EEMEM;
uint16_t (*getCurrentData_EEPROM)() EEMEM;
uint16_t (*getTrigger_EEPROM)() EEMEM;
uint16_t dataLogIt_EEPROM EEMEM;


void DataLogger__eepromInit (void)
{
	mode = eeprom_read_byte(&dlMode_EEPROM);
	getCurrentData = eeprom_read_word(&getCurrentData_EEPROM);
	getTrigger = eeprom_read_word(&getTrigger_EEPROM);
	dataLogIt = eeprom_read_word(&dataLogIt_EEPROM);

	if (dataLogIt != 0xFFFF)
	{
		eeprom_read_block(dataLog, &dataLog_EEPROM, 2 * dataLogIt);
	}

	if (mode == 0xFF)
	{
		mode = DL_MODE__IDLE;
	}
}


static void DataLogger__eepromStorage (void)
{
	eeprom_update_byte(&dlMode_EEPROM, mode);
	eeprom_update_word(&getCurrentData_EEPROM, getCurrentData);
	eeprom_update_word(&getTrigger_EEPROM, getTrigger);
	eeprom_update_word(&dataLogIt_EEPROM, dataLogIt);
	eeprom_update_block(dataLog, &dataLog_EEPROM, 2 * dataLogIt);
}


void DataLogger__x10 (void)
{
	switch (mode)
	{
		case DL_MODE__IDLE:
		{
			/* nothing to do */
			break;
		}

		case DL_MODE__MEASUREMENT:
		{
			if (getTrigger() == TRUE)
			{
				dataLog[dataLogIt] = getCurrentData();

				if (dataLogIt >= MAX_MEASUREMENT_POINTS)
				{
					mode = DL_MODE__PAUSE;
				}

				dataLogIt++;

				/* test */
				toggle(TEST_LED_PORT, TEST_LED_PIN);
			}
			break;
		}

		case DL_MODE__PAUSE:
		{
			/* nothing to do */
			break;
		}
	}

	DataLogger__eepromStorage();
}


void DataLogger__startMeasure (uint16_t (*getValue)(), uint8_t (*trigger)())
{
	if (mode == DL_MODE__IDLE)
	{
		mode = DL_MODE__MEASUREMENT;

		dataLogIt = 0;
		getCurrentData = getValue;
		getTrigger = trigger;
	}
}


void DataLogger__stopMeasure (void)
{
	if (mode == DL_MODE__MEASUREMENT)
	{
		mode = DL_MODE__IDLE;
	}
}


void DataLogger__pauseMeasure (void)
{
	if (mode == DL_MODE__MEASUREMENT)
	{
		mode = DL_MODE__PAUSE;
	}
}


void DataLogger__resumeMeasure (void)
{
	if (mode == DL_MODE__PAUSE)
	{
		mode = DL_MODE__MEASUREMENT;
	}
}


uint16_t DataLogger__getNumberOfStoredValues (void)
{
	return (dataLogIt); /* dataLogIt already incremented */
}


uint16_t DataLogger__getStoredValue (uint16_t index)
{
	/* index: [1 .. DataLogger__getNumberOfStoredValues] */
	return (dataLog[index - 1]);
}
