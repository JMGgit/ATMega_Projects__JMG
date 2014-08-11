/*
 * DataLogger.c
 *
 *  Created on: 28.05.2014
 *      Author: Jean-Martin George
 */


#include "DataLogger.h"


#define DL_MODE__IDLE				1
#define DL_MODE__MEASUREMENT		2


static uint8_t timeCount;
static uint8_t timePrev;
static measurementInfos_t dataLogMeasurementInfos[DATA_LOGGER_MEASURES_NB];
static uint16_t dataLog[MAX_MEASUREMENT_POINTS];
static uint8_t mode;
static uint16_t (*getCurrentData)();
static uint16_t dataLogIt;
static uint16_t dataLogIndexTable[DATA_LOGGER_MEASURES_NB];
static uint8_t measIndex;

uint8_t measIndex_EEPROM EEMEM;
uint16_t dataLogIt_EEPROM EEMEM;
measurementInfos_t dataLogMeasurementInfos_EEPROM[DATA_LOGGER_MEASURES_NB] EEMEM;
uint16_t dataLogIndexTable_EEPROM[DATA_LOGGER_MEASURES_NB] EEMEM;
uint16_t dataLog_EEPROM[MAX_MEASUREMENT_POINTS] EEMEM;


void DataLogger__eepromInit (void)
{
	mode = DL_MODE__IDLE;
	dataLogIt = eeprom_read_word(&dataLogIt_EEPROM);
	measIndex = eeprom_read_byte(&measIndex_EEPROM);

	if ((dataLogIt != 0xFFFF) && (dataLogIt != 0))
	{
		eeprom_read_block(dataLog, &dataLog_EEPROM, 2 * dataLogIt);
		eeprom_read_block(dataLogIndexTable, &dataLogIndexTable_EEPROM, 2 * DATA_LOGGER_MEASURES_NB);
		eeprom_read_block(dataLogMeasurementInfos, &dataLogMeasurementInfos_EEPROM, sizeof(measurementInfos_t) * DATA_LOGGER_MEASURES_NB);
	}
	else
	{
		dataLogIt = 0;
		measIndex = 0;
	}
}


static void DataLogger__eepromStorage (void)
{
	eeprom_update_word(&dataLogIt_EEPROM, dataLogIt);
	eeprom_update_block(dataLog, &dataLog_EEPROM, 2 * dataLogIt);
	eeprom_update_byte(&measIndex_EEPROM, measIndex);
	eeprom_update_block(dataLogIndexTable, &dataLogIndexTable_EEPROM, 2 * DATA_LOGGER_MEASURES_NB);
	eeprom_update_block(dataLogMeasurementInfos, &dataLogMeasurementInfos_EEPROM, sizeof(measurementInfos_t) * DATA_LOGGER_MEASURES_NB);
}


static uint8_t DataLogger__getTrigger (void)
{
	uint8_t trigger = FALSE;

	if (timeCount == Mode_SetupMeasurement__getInterval())
	{
		trigger = TRUE;
		timeCount = 0;
	}
	else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_DAY) && (Clock__getDate() != timePrev)
			&& 	(Clock__getHours() == dataLogMeasurementInfos[measIndex - 1].hourStart)
			&&	(Clock__getMinutes() == dataLogMeasurementInfos[measIndex - 1].minuteStart)
			&& 	(Clock__getSeconds() == dataLogMeasurementInfos[measIndex - 1].secondStart)
	)
	{
		timePrev = Clock__getDate();
		timeCount++;
	}
	else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_HOUR) && (Clock__getHours() != timePrev)
			&&	(Clock__getMinutes() == dataLogMeasurementInfos[measIndex - 1].minuteStart)
			&& 	(Clock__getSeconds() == dataLogMeasurementInfos[measIndex - 1].secondStart)
	)
	{
		timePrev = Clock__getHours();
		timeCount++;
	}
	else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_MINUTE) && (Clock__getMinutes() != timePrev)
			&& 	(Clock__getSeconds() == dataLogMeasurementInfos[measIndex - 1].secondStart)
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

	return trigger;
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
			if ((dataLogIt >= MAX_MEASUREMENT_POINTS) || (measIndex >= DATA_LOGGER_MEASURES_NB))
			{
				mode = DL_MODE__IDLE;
			}
			else
			{
				if (DataLogger__getTrigger() == TRUE)
				{
					dataLog[dataLogIt] = getCurrentData();
					dataLogIt++;
				}
			}
			break;
		}
	}

	DataLogger__eepromStorage();
}


void DataLogger__startMeasure (uint16_t (*getValue)())
{
	if ((measIndex < DATA_LOGGER_MEASURES_NB) && (dataLogIt < MAX_MEASUREMENT_POINTS))
	{
		mode = DL_MODE__MEASUREMENT;
		getCurrentData = getValue;

		dataLogIndexTable[measIndex] = dataLogIt;
		dataLogMeasurementInfos[measIndex].yearStart = Clock__getYear();
		dataLogMeasurementInfos[measIndex].monthStart = Clock__getMonth();
		dataLogMeasurementInfos[measIndex].dateStart = Clock__getDate();
		dataLogMeasurementInfos[measIndex].hourStart = Clock__getHours();
		dataLogMeasurementInfos[measIndex].minuteStart = Clock__getMinutes();
		dataLogMeasurementInfos[measIndex].secondStart = Clock__getSeconds();
		dataLogMeasurementInfos[measIndex].interval = Mode_SetupMeasurement__getInterval();
		dataLogMeasurementInfos[measIndex].unit = Mode_SetupMeasurement__getUnit();
		measIndex++;

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
	}
}


void DataLogger__stopMeasure (void)
{
	mode = DL_MODE__IDLE;
}


uint16_t DataLogger__getNumberOfStoredValuesOfMeasure (uint8_t measureNumber)
{
	uint16_t retVal = 0;

	if (measureNumber < measIndex)
	{
		retVal =  dataLogIndexTable[measureNumber] - dataLogIndexTable[measureNumber - 1];
	}
	else if (measureNumber == measIndex)
	{
		retVal = dataLogIt - dataLogIndexTable[measureNumber - 1];
	}

	return retVal;
}


uint16_t DataLogger__getFreeMeasurementPoints (void)
{
	return (MAX_MEASUREMENT_POINTS - dataLogIt); /* dataLogIt already incremented */
}


uint16_t DataLogger__getStoredValueOfMeasure (uint8_t measureNumber, uint16_t index)
{
	if ((measureNumber <= measIndex) && (index < DataLogger__getNumberOfStoredValuesOfMeasure(measureNumber)))
	{
		return (dataLog[(index + dataLogIndexTable[measureNumber - 1])]);
	}
	else
	{
		return 0;
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


void DataLogger__getLastValueWithTime (uint16_t *lastMeasYear, uint8_t *lastMeasMonth, uint8_t *lastMeasDate, uint8_t *lastMeasHour, uint8_t *lastMeasMin, uint8_t *lastMeasSec, uint16_t *lastData)
{
	DataLogger__getValueWithTime(measIndex, DataLogger__getNumberOfStoredValuesOfMeasure(measIndex) - 1, lastMeasYear, lastMeasMonth, lastMeasDate, lastMeasHour, lastMeasMin, lastMeasSec, lastData);
}


void DataLogger__getValueWithTime (uint8_t measureNumber, uint16_t index, uint16_t *measYear, uint8_t *measMonth, uint8_t *measDate, uint8_t *measHour, uint8_t *measMin, uint8_t *measSec, uint16_t *data)
{
	uint8_t interval, unit;

	if ((measureNumber <= measIndex) && (index < DataLogger__getNumberOfStoredValuesOfMeasure(measureNumber)))
	{
		DataLogger__getIntervalAndUnitOfMeasure(measureNumber, &interval, &unit);

		if (unit == MEASUREMENT_UNIT_DAY)
		{
			*measYear = dataLogMeasurementInfos[measureNumber - 1].yearStart;
			*measMonth = (dataLogMeasurementInfos[measureNumber - 1].monthStart + (dataLogMeasurementInfos[measureNumber - 1].dateStart + (index * interval)) / 30) % 12;
			*measDate = (dataLogMeasurementInfos[measureNumber - 1].dateStart + (index * interval)) % 30;
			*measHour = dataLogMeasurementInfos[measureNumber - 1].hourStart;
			*measMin = dataLogMeasurementInfos[measureNumber - 1].minuteStart;
			*measSec = dataLogMeasurementInfos[measureNumber - 1].secondStart;
		}
		else if (unit == MEASUREMENT_UNIT_HOUR)
		{
			*measYear = dataLogMeasurementInfos[measureNumber - 1].yearStart;
			*measMonth = (dataLogMeasurementInfos[measureNumber - 1].monthStart + (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * interval)) / 2592000) % 12;
			*measDate = (dataLogMeasurementInfos[measureNumber - 1].dateStart + (dataLogMeasurementInfos[measureNumber - 1].hourStart + (index * interval)) / 24) % 30;
			*measHour = (dataLogMeasurementInfos[measureNumber - 1].hourStart + (index * interval)) % 24;
			*measMin = dataLogMeasurementInfos[measureNumber - 1].minuteStart;
			*measSec = dataLogMeasurementInfos[measureNumber - 1].secondStart;
		}
		else if (unit == MEASUREMENT_UNIT_MINUTE)
		{
			*measYear = dataLogMeasurementInfos[measureNumber - 1].yearStart;
			*measMonth = (dataLogMeasurementInfos[measureNumber - 1].monthStart + (dataLogMeasurementInfos[measureNumber - 1].minuteStart + (index * interval)) / 43200) % 12;
			*measDate = (dataLogMeasurementInfos[measureNumber - 1].dateStart + (dataLogMeasurementInfos[measureNumber - 1].minuteStart + (index * interval)) / 1440) % 30;
			*measHour = (dataLogMeasurementInfos[measureNumber - 1].hourStart + (dataLogMeasurementInfos[measureNumber - 1].minuteStart + (index * interval)) / 60) % 24;
			*measMin = (dataLogMeasurementInfos[measureNumber - 1].minuteStart + (index * interval)) % 60;
			*measSec = dataLogMeasurementInfos[measureNumber - 1].secondStart;
		}
		else if (unit == MEASUREMENT_UNIT_SECOND)
		{
			*measYear = dataLogMeasurementInfos[measureNumber - 1].yearStart;
			*measMonth = (dataLogMeasurementInfos[measureNumber - 1].monthStart + (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * interval)) / 2592000) % 12;
			*measDate = (dataLogMeasurementInfos[measureNumber - 1].dateStart + (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * interval)) / 86400) % 30;
			*measHour = (dataLogMeasurementInfos[measureNumber - 1].hourStart + (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * interval)) / 3600) % 24;
			*measMin = (dataLogMeasurementInfos[measureNumber - 1].minuteStart + (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * interval)) / 60) % 60;
			*measSec = (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * interval)) % 60;
		}
	}
	else
	{
		*measYear = 0;
		*measMonth = 0;
		*measDate = 0;
		*measHour = 0;
		*measMin = 0;
		*measSec = 0;
	}

	*data = DataLogger__getStoredValueOfMeasure(measureNumber, index);
}


void DataLogger__getStartTimeOfMeasure (uint8_t measureNumber, uint16_t *measYear, uint8_t *measMonth, uint8_t *measDate, uint8_t *measHour, uint8_t *measMin, uint8_t *measSec)
{
	if (measureNumber <= measIndex)
	{
		*measYear = dataLogMeasurementInfos[measureNumber - 1].yearStart;
		*measMonth = dataLogMeasurementInfos[measureNumber - 1].monthStart;
		*measDate = dataLogMeasurementInfos[measureNumber - 1].dateStart;
		*measHour = dataLogMeasurementInfos[measureNumber - 1].hourStart;
		*measMin = dataLogMeasurementInfos[measureNumber - 1].minuteStart;
		*measSec = dataLogMeasurementInfos[measureNumber - 1].secondStart;
	}
	else
	{
		*measYear = 0;
		*measMonth = 0;
		*measDate = 0;
		*measHour = 0;
		*measMin = 0;
		*measSec = 0;
	}
}


void DataLogger__getIntervalAndUnitOfMeasure (uint8_t measureNumber, uint8_t *interval, uint8_t *unit)
{
	if (measureNumber <= measIndex)
	{
		*interval = dataLogMeasurementInfos[measureNumber - 1].interval;
		*unit = dataLogMeasurementInfos[measureNumber - 1].unit;
	}
	else
	{
		*interval = 0;
		*unit = 0;;
	}
}


uint8_t DataLogger__getNumberOfMeasures (void)
{
	return measIndex;
}


void DataLogger__clearMeasure (uint8_t measureNumber)
{
	/* TODO: not tested!! */
	uint16_t nbOfstoredValues, indexOfValues;

	if (measureNumber <= measIndex)
	{
		/* 1. delete measurement data */
		nbOfstoredValues = DataLogger__getNumberOfStoredValuesOfMeasure(measureNumber);
		indexOfValues = dataLogIndexTable[measureNumber - 1];
		memset(&dataLogMeasurementInfos[measureNumber - 1], 0, sizeof(measurementInfos_t));
		memset(&dataLog[indexOfValues], 0, nbOfstoredValues * 2);
		dataLogIndexTable[measureNumber - 1] = 0;

		/* 2. move other measurement data */
		if (measureNumber < DATA_LOGGER_MEASURES_NB)
		{
			memmove(&dataLogMeasurementInfos[measureNumber - 1], &dataLogMeasurementInfos[measureNumber], sizeof(measurementInfos_t) * (measIndex - measureNumber));
			memmove(&dataLog[indexOfValues], &dataLog[dataLogIndexTable[measureNumber]], sizeof(measurementInfos_t));
		}

		/* 3. update measurement index and data iterator */
		measIndex = measIndex - 1;
		dataLogIt = dataLogIt - nbOfstoredValues;

		/* 4. update EEPROM */
		DataLogger__eepromStorage();
	}
}


void DataLogger__clearAll (void)
{
	measIndex = 0;
	dataLogIt = 0;
	DataLogger__eepromStorage();
}
