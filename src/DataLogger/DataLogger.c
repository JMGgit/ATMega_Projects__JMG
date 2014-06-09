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


static uint16_t dataLog[MAX_MEASUREMENT_POINTS];
uint16_t dataLog_EEPROM[MAX_MEASUREMENT_POINTS] EEMEM;
static uint8_t mode;
static uint16_t (*getCurrentData)();
static uint8_t (*getTrigger)();
static uint16_t dataLogIt;
static uint16_t dataLogIndexTable[DATA_LOGGER_MEASURES_NB];
static uint8_t mesIndex;
uint16_t dataLogIt_EEPROM EEMEM;
uint8_t mesIndex_EEPROM EEMEM;
uint16_t dataLogIndexTable_EEPROM[DATA_LOGGER_MEASURES_NB] EEMEM;


void DataLogger__eepromInit (void)
{
	mode = DL_MODE__IDLE;
	dataLogIt = eeprom_read_word(&dataLogIt_EEPROM);
	mesIndex = eeprom_read_byte(&mesIndex_EEPROM);

	if ((dataLogIt != 0xFFFF) && (dataLogIt != 0))
	{
		eeprom_read_block(dataLog, &dataLog_EEPROM, 2 * dataLogIt);
		eeprom_read_block(dataLogIndexTable, &dataLogIndexTable_EEPROM, 2 * DATA_LOGGER_MEASURES_NB);
	}
	else
	{
		dataLogIt = 0;
		mesIndex = 0;
	}
}


static void DataLogger__eepromStorage (void)
{
	eeprom_update_word(&dataLogIt_EEPROM, dataLogIt);
	eeprom_update_block(dataLog, &dataLog_EEPROM, 2 * dataLogIt);
	eeprom_update_byte(&mesIndex_EEPROM, mesIndex);
	eeprom_update_block(dataLogIndexTable, &dataLogIndexTable_EEPROM, 2 * DATA_LOGGER_MEASURES_NB);
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
					mode = DL_MODE__IDLE;
				}

				dataLogIt++;

				/* test */
				toggle(TEST_LED_PORT, TEST_LED_PIN);
			}
			break;
		}
	}

	DataLogger__eepromStorage();
}


void DataLogger__startMeasure (uint16_t (*getValue)(), uint8_t (*trigger)())
{
	if (mesIndex < DATA_LOGGER_MEASURES_NB)
	{
		mode = DL_MODE__MEASUREMENT;
		getCurrentData = getValue;
		getTrigger = trigger;
		dataLogIndexTable[mesIndex] = dataLogIt;
		mesIndex++;
	}
}


void DataLogger__stopMeasure (void)
{
	mode = DL_MODE__IDLE;
}


uint16_t DataLogger__getNumberOfStoredValuesOfMeasure (uint8_t measureNumber)
{
	return (dataLogIt - dataLogIndexTable[measureNumber] - 1); /* dataLogIt already incremented */
}


uint16_t DataLogger__getStoredValueOfMeasure (uint8_t measureNumber, uint16_t index)
{
	/* index: [1 .. DataLogger__getNumberOfStoredValues] */
	return (dataLog[(index + dataLogIndexTable[measureNumber])]);
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
