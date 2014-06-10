/*
 * DataLogger.c
 *
 *  Created on: 28.05.2014
 *      Author: Jean-Martin George
 */


#include "DataLogger.h"
#include "../Modes/Mode_Setup_Measurement.h"


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
static uint8_t mesIndex;

uint8_t mesIndex_EEPROM EEMEM;
uint16_t dataLogIt_EEPROM EEMEM;
measurementInfos_t dataLogMeasurementInfos_EEPROM[DATA_LOGGER_MEASURES_NB] EEMEM;
uint16_t dataLogIndexTable_EEPROM[DATA_LOGGER_MEASURES_NB] EEMEM;
uint16_t dataLog_EEPROM[MAX_MEASUREMENT_POINTS] EEMEM;


void DataLogger__eepromInit (void)
{
	mode = DL_MODE__IDLE;
	dataLogIt = eeprom_read_word(&dataLogIt_EEPROM);
	mesIndex = eeprom_read_byte(&mesIndex_EEPROM);

	if ((dataLogIt != 0xFFFF) && (dataLogIt != 0))
	{
		eeprom_read_block(dataLog, &dataLog_EEPROM, 2 * dataLogIt);
		eeprom_read_block(dataLogIndexTable, &dataLogIndexTable_EEPROM, 2 * DATA_LOGGER_MEASURES_NB);
		eeprom_read_block(dataLogMeasurementInfos, &dataLogMeasurementInfos_EEPROM, sizeof(measurementInfos_t) * DATA_LOGGER_MEASURES_NB);
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
			&& 	(Clock__getHours() == dataLogMeasurementInfos[mesIndex - 1].hourStart)
			&&	(Clock__getMinutes() == dataLogMeasurementInfos[mesIndex - 1].minuteStart)
			&& 	(Clock__getSeconds() == dataLogMeasurementInfos[mesIndex - 1].secondStart)
	)
	{
		timePrev = Clock__getDate();
		timeCount++;
	}
	else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_HOUR) && (Clock__getHours() != timePrev)
			&&	(Clock__getMinutes() == dataLogMeasurementInfos[mesIndex - 1].minuteStart)
			&& 	(Clock__getSeconds() == dataLogMeasurementInfos[mesIndex - 1].secondStart)
	)
	{
		timePrev = Clock__getHours();
		timeCount++;
	}
	else if (	(Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_MINUTE) && (Clock__getMinutes() != timePrev)
			&& 	(Clock__getSeconds() == dataLogMeasurementInfos[mesIndex - 1].secondStart)
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
			if ((dataLogIt >= MAX_MEASUREMENT_POINTS) || (mesIndex >= DATA_LOGGER_MEASURES_NB))
			{
				mode = DL_MODE__IDLE;
			}
			else
			{
				if (DataLogger__getTrigger() == TRUE)
				{
					dataLog[dataLogIt] = getCurrentData();
					dataLogIt++;

					/* test */
					toggle(TEST_LED_PORT, TEST_LED_PIN);
				}
			}
			break;
		}
	}

	DataLogger__eepromStorage();
}


void DataLogger__startMeasure (uint16_t (*getValue)())
{
	if ((mesIndex < DATA_LOGGER_MEASURES_NB) && (dataLogIt < MAX_MEASUREMENT_POINTS))
	{
		mode = DL_MODE__MEASUREMENT;
		getCurrentData = getValue;

		dataLogIndexTable[mesIndex] = dataLogIt;
		dataLogMeasurementInfos[mesIndex].yearStart = Clock__getYear();
		dataLogMeasurementInfos[mesIndex].monthStart = Clock__getMonth();
		dataLogMeasurementInfos[mesIndex].dateStart = Clock__getDate();
		dataLogMeasurementInfos[mesIndex].hourStart = Clock__getHours();
		dataLogMeasurementInfos[mesIndex].minuteStart = Clock__getMinutes();
		dataLogMeasurementInfos[mesIndex].secondStart = Clock__getSeconds();
		mesIndex++;

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

	if (measureNumber < mesIndex)
	{
		retVal =  dataLogIndexTable[measureNumber] - dataLogIndexTable[measureNumber - 1];
	}
	else if (measureNumber == mesIndex)
	{
		retVal = dataLogIt - dataLogIndexTable[measureNumber - 1];
	}

	return retVal;
}


uint16_t DataLogger__getStoredValueOfMeasure (uint8_t measureNumber, uint16_t index)
{
	if ((measureNumber <= mesIndex) && (index < DataLogger__getNumberOfStoredValuesOfMeasure(measureNumber)))
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


void DataLogger__getLastValueWithTime (uint8_t *lastMesMonth, uint8_t *lastMesDate, uint8_t *lastMesHour, uint8_t *lastMesMin, uint8_t *lastMesSec, uint16_t *lastData)
{
	DataLogger__getValueWithTime(mesIndex, DataLogger__getNumberOfStoredValuesOfMeasure(mesIndex) - 1, lastMesMonth, lastMesDate, lastMesHour, lastMesMin, lastMesSec, lastData);
}


void DataLogger__getValueWithTime (uint8_t measureNumber, uint8_t index, uint8_t *mesMonth, uint8_t *mesDate, uint8_t *mesHour, uint8_t *mesMin, uint8_t *mesSec, uint16_t *data)
{
	if ((measureNumber <= mesIndex) && (index < DataLogger__getNumberOfStoredValuesOfMeasure(measureNumber)))
	{
		if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_DAY)
		{
			*mesMonth = (dataLogMeasurementInfos[measureNumber - 1].monthStart + (dataLogMeasurementInfos[measureNumber - 1].dateStart + (index * Mode_SetupMeasurement__getInterval())) / 30) % 12;
			*mesDate = (dataLogMeasurementInfos[measureNumber - 1].dateStart + (index * Mode_SetupMeasurement__getInterval())) % 30;
			*mesHour = dataLogMeasurementInfos[measureNumber - 1].hourStart;
			*mesMin = dataLogMeasurementInfos[measureNumber - 1].minuteStart;
			*mesSec = dataLogMeasurementInfos[measureNumber - 1].secondStart;
		}
		else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_HOUR)
		{
			*mesMonth = (dataLogMeasurementInfos[measureNumber - 1].monthStart + (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * Mode_SetupMeasurement__getInterval())) / 2592000) % 60;
			*mesDate = (dataLogMeasurementInfos[measureNumber - 1].dateStart + (dataLogMeasurementInfos[measureNumber - 1].hourStart + (index * Mode_SetupMeasurement__getInterval())) / 24) % 30;
			*mesHour = (dataLogMeasurementInfos[measureNumber - 1].hourStart + (index * Mode_SetupMeasurement__getInterval())) % 24;
			*mesMin = dataLogMeasurementInfos[measureNumber - 1].minuteStart;
			*mesSec = dataLogMeasurementInfos[measureNumber - 1].secondStart;
		}
		else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_MINUTE)
		{
			*mesMonth = (dataLogMeasurementInfos[measureNumber - 1].monthStart + (dataLogMeasurementInfos[measureNumber - 1].minuteStart + (index * Mode_SetupMeasurement__getInterval())) / 43200) % 60;
			*mesDate = (dataLogMeasurementInfos[measureNumber - 1].dateStart + (dataLogMeasurementInfos[measureNumber - 1].minuteStart + (index * Mode_SetupMeasurement__getInterval())) / 1440) % 30;
			*mesHour = (dataLogMeasurementInfos[measureNumber - 1].hourStart + (dataLogMeasurementInfos[measureNumber - 1].minuteStart + (index * Mode_SetupMeasurement__getInterval())) / 60) % 24;
			*mesMin = (dataLogMeasurementInfos[measureNumber - 1].minuteStart + (index * Mode_SetupMeasurement__getInterval())) % 60;
			*mesSec = dataLogMeasurementInfos[measureNumber - 1].secondStart;
		}
		else if (Mode_SetupMeasurement__getUnit() == MEASUREMENT_UNIT_SECOND)
		{
			*mesMonth = (dataLogMeasurementInfos[measureNumber - 1].monthStart + (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * Mode_SetupMeasurement__getInterval())) / 2592000) % 60;
			*mesDate = (dataLogMeasurementInfos[measureNumber - 1].dateStart + (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * Mode_SetupMeasurement__getInterval())) / 86400) % 30;
			*mesHour = (dataLogMeasurementInfos[measureNumber - 1].hourStart + (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * Mode_SetupMeasurement__getInterval())) / 3600) % 24;
			*mesMin = (dataLogMeasurementInfos[measureNumber - 1].minuteStart + (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * Mode_SetupMeasurement__getInterval())) / 60) % 60;
			*mesSec = (dataLogMeasurementInfos[measureNumber - 1].secondStart + (index * Mode_SetupMeasurement__getInterval())) % 60;
		}
	}
	else
	{
		*mesMonth = 0;
		*mesDate = 0;
		*mesHour = 0;
		*mesMin = 0;
		*mesSec = 0;
	}

	*data = DataLogger__getStoredValueOfMeasure(measureNumber, index);
}


void DataLogger__getStartTimeOfMeasure (uint8_t measureNumber, uint8_t *mesMonth, uint8_t *mesDate, uint8_t *mesHour, uint8_t *mesMin, uint8_t *mesSec)
{
	if (measureNumber <= mesIndex)
	{
		*mesMonth = dataLogMeasurementInfos[measureNumber - 1].monthStart;
		*mesDate = dataLogMeasurementInfos[measureNumber - 1].dateStart;
		*mesHour = dataLogMeasurementInfos[measureNumber - 1].hourStart;
		*mesMin = dataLogMeasurementInfos[measureNumber - 1].minuteStart;
		*mesSec = dataLogMeasurementInfos[measureNumber - 1].secondStart;
	}
	else
	{
		*mesMonth = 0;
		*mesDate = 0;
		*mesHour = 0;
		*mesMin = 0;
		*mesSec = 0;
	}
}

uint8_t DataLogger__getNumberOfMeasures (void)
{
	return mesIndex;
}

