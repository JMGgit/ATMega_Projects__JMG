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
static uint8_t (*getCurrentData)(uint16_t *data);
static uint16_t dataLogIt;
static uint16_t dataLogIndexTable[DATA_LOGGER_MEASURES_NB];
static uint8_t measIndex;

uint8_t measIndex_EEPROM EEMEM;
uint16_t dataLogIt_EEPROM EEMEM;
measurementInfos_t dataLogMeasurementInfos_EEPROM[DATA_LOGGER_MEASURES_NB] EEMEM;
uint16_t dataLogIndexTable_EEPROM[DATA_LOGGER_MEASURES_NB] EEMEM;
uint16_t dataLog_EEPROM[MAX_MEASUREMENT_POINTS] EEMEM;

static uint8_t daysInYear[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static uint8_t daysInLeapYear[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


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
		eeprom_update_word(&dataLogIt_EEPROM, dataLogIt);
		eeprom_update_byte(&measIndex_EEPROM, measIndex);
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

	if (timeCount == dataLogMeasurementInfos[measIndex - 1].interval)
	{
		trigger = TRUE;
		timeCount = 0;
	}
	else if (	(dataLogMeasurementInfos[measIndex - 1].unit == MEASUREMENT_UNIT_DAY) && (Clock__getDate() != timePrev)
			&& 	(Clock__getHours() == dataLogMeasurementInfos[measIndex - 1].hourStart)
			&&	(Clock__getMinutes() == dataLogMeasurementInfos[measIndex - 1].minuteStart)
			&& 	(Clock__getSeconds() == dataLogMeasurementInfos[measIndex - 1].secondStart)
	)
	{
		timePrev = Clock__getDate();
		timeCount++;
	}
	else if (	(dataLogMeasurementInfos[measIndex - 1].unit == MEASUREMENT_UNIT_HOUR) && (Clock__getHours() != timePrev)
			&&	(Clock__getMinutes() == dataLogMeasurementInfos[measIndex - 1].minuteStart)
			&& 	(Clock__getSeconds() == dataLogMeasurementInfos[measIndex - 1].secondStart)
	)
	{
		timePrev = Clock__getHours();
		timeCount++;
	}
	else if (	(dataLogMeasurementInfos[measIndex - 1].unit == MEASUREMENT_UNIT_MINUTE) && (Clock__getMinutes() != timePrev)
			&& 	(Clock__getSeconds() == dataLogMeasurementInfos[measIndex - 1].secondStart)
	)
	{
		timePrev = Clock__getMinutes();
		timeCount++;
	}
	else if ((dataLogMeasurementInfos[measIndex - 1].unit == MEASUREMENT_UNIT_SECOND) && (Clock__getSeconds() != timePrev))
	{
		timePrev = Clock__getSeconds();
		timeCount++;
	}

	return trigger;
}


void DataLogger__x10 (void)
{
	uint16_t value;
	static uint8_t waitForTrigger= FALSE;

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
				if ((DataLogger__getTrigger() == TRUE) || (waitForTrigger == TRUE))
				{
					if (getCurrentData(&value) == E_OK)
					{
						dataLog[dataLogIt] = value;
						dataLogMeasurementInfos[measIndex - 1].min = Temperature__getMinRawValue(dataLog[dataLogIt], dataLogMeasurementInfos[measIndex - 1].min);
						dataLogMeasurementInfos[measIndex - 1].max = Temperature__getMaxRawValue(dataLog[dataLogIt], dataLogMeasurementInfos[measIndex - 1].max);
						dataLogIt++;
						waitForTrigger = FALSE;
					}
					else
					{
						waitForTrigger = TRUE;
					}
				}
			}
			break;
		}
	}

	DataLogger__eepromStorage();
}


void DataLogger__startMeasure (uint8_t (*getValue)(uint16_t*))
{
	if ((measIndex < DATA_LOGGER_MEASURES_NB) && (dataLogIt < MAX_MEASUREMENT_POINTS))
	{
		mode = DL_MODE__MEASUREMENT;
		getCurrentData = getValue;
		measIndex++;

		dataLogIndexTable[measIndex - 1] = dataLogIt;
		dataLogMeasurementInfos[measIndex - 1].yearStart = 2000 + Clock__getYear();
		dataLogMeasurementInfos[measIndex - 1].monthStart = Clock__getMonth();
		dataLogMeasurementInfos[measIndex - 1].dateStart = Clock__getDate();
		dataLogMeasurementInfos[measIndex - 1].hourStart = Clock__getHours();
		dataLogMeasurementInfos[measIndex - 1].minuteStart = Clock__getMinutes();
		dataLogMeasurementInfos[measIndex - 1].secondStart = Clock__getSeconds();
		dataLogMeasurementInfos[measIndex - 1].interval = Mode_SetupMeasurement__getInterval();
		dataLogMeasurementInfos[measIndex - 1].unit = Mode_SetupMeasurement__getUnit();
		dataLogMeasurementInfos[measIndex - 1].min = 0x7D0;
		dataLogMeasurementInfos[measIndex - 1].max = 0xFC90;

		timeCount = dataLogMeasurementInfos[measIndex - 1].interval;

		if (dataLogMeasurementInfos[measIndex - 1].unit == MEASUREMENT_UNIT_DAY)
		{
			timePrev = Clock__getDate();
		}
		else if (dataLogMeasurementInfos[measIndex - 1].unit == MEASUREMENT_UNIT_HOUR)
		{
			timePrev = Clock__getHours();
		}
		else if (dataLogMeasurementInfos[measIndex - 1].unit == MEASUREMENT_UNIT_MINUTE)
		{
			timePrev = Clock__getMinutes();
		}
		else if (dataLogMeasurementInfos[measIndex - 1].unit == MEASUREMENT_UNIT_SECOND)
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


static uint8_t DataLogger__isLeapYear (uint16_t year)
{
	if (((year % 400) == 0) || (((year % 100) != 0) && ((year % 4) == 0)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


static uint8_t DataLogger__getNumberOfDays (uint16_t year, uint8_t month)
{
	if (DataLogger__isLeapYear(year))
	{
		return daysInLeapYear[month];
	}
	else
	{
		return daysInYear[month];
	}
}


static void DataLogger__getTime1Plus2 (	uint16_t year_1, uint8_t month_1, uint8_t date_1, uint8_t hour_1, uint8_t min_1, uint8_t sec_1,
										uint16_t date_2, uint16_t hour_2, uint16_t min_2, uint16_t sec_2,
										uint16_t *year_res, uint8_t *month_res, uint8_t *date_res, uint8_t *hour_res, uint8_t *min_res, uint8_t *sec_res)
{
	uint16_t days, currentYear;
	uint8_t currentMonth, currentDate;

	*sec_res = (sec_1 + sec_2) % 60;
	*min_res = (min_1 + min_2 + ((sec_1 + sec_2) / 60)) % 60;
	*hour_res = (hour_1 + hour_2 + ((min_1 + min_2 + ((sec_1 + sec_2) / 60)) / 60)) % 24;
	currentDate = date_1;
	currentYear = year_1;
	currentMonth = month_1;
	days = date_2 + ((hour_2 + ((min_2 + (sec_2 / 60)) / 60)) / 24);

	if (	(*hour_res < hour_1)
		|| 	((*hour_res == hour_1) && (*min_res < min_1))
		||	((*hour_res == hour_1) && (*min_res == min_1) && (*sec_res < sec_1))
	)
	{
		days++;
	}

	while ((int)(days - (DataLogger__getNumberOfDays(currentYear, currentMonth) - currentDate)) > 0)
	{
		days = days - (DataLogger__getNumberOfDays(currentYear, currentMonth) - currentDate + 1);
		currentDate = 1;

		if (currentMonth < 12)
		{
			currentMonth++;
		}
		else
		{
			currentMonth = 1;
			currentYear++;
		}
	}

	if (days > 0)
	{
		currentDate += (uint8_t)days;
	}

	*date_res = currentDate;
	*month_res = currentMonth;
	*year_res = currentYear;
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

		DataLogger__getTime1Plus2(	dataLogMeasurementInfos[measureNumber - 1].yearStart,
									dataLogMeasurementInfos[measureNumber - 1].monthStart,
									dataLogMeasurementInfos[measureNumber - 1].dateStart,
									dataLogMeasurementInfos[measureNumber - 1].hourStart,
									dataLogMeasurementInfos[measureNumber - 1].minuteStart,
									dataLogMeasurementInfos[measureNumber - 1].secondStart,
									(index * interval) * (unit == MEASUREMENT_UNIT_DAY),
									(index * interval) * (unit == MEASUREMENT_UNIT_HOUR),
									(index * interval) * (unit == MEASUREMENT_UNIT_MINUTE),
									(index * interval) * (unit == MEASUREMENT_UNIT_SECOND),
									measYear, measMonth, measDate, measHour, measMin, measSec
								);
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


void DataLogger__getMinValueString (uint8_t measureNumber, char *buffer)
{
	uint8_t negative, t_int, t_frac;

	DataLogger__getMinValues (measureNumber, &negative, &t_int, &t_frac);
	Temperature__getValueString(negative, t_int, t_frac, buffer);
}


void DataLogger__getMinValues (uint8_t measureNumber, uint8_t *negative, uint8_t *t_int, uint8_t *t_frac)
{
	Temperature__getValuesFromRaw(dataLogMeasurementInfos[measureNumber - 1].min, negative, t_int, t_frac);
}


void DataLogger__getMaxValueString (uint8_t measureNumber, char *buffer)
{
	uint8_t negative, t_int, t_frac;

	DataLogger__getMaxValues (measureNumber, &negative, &t_int, &t_frac);
	Temperature__getValueString(negative, t_int, t_frac, buffer);
}


void DataLogger__getMaxValues (uint8_t measureNumber, uint8_t *negative, uint8_t *t_int, uint8_t *t_frac)
{
	Temperature__getValuesFromRaw(dataLogMeasurementInfos[measureNumber - 1].max, negative, t_int, t_frac);
}


void DataLogger__getAverageValueString (uint8_t measureNumber, char *buffer)
{
	uint8_t negative_a, t_int_a, t_frac_a;

	DataLogger__getAverageValues(measureNumber, &negative_a, &t_int_a, &t_frac_a);
	Temperature__getValueString(negative_a, t_int_a, t_frac_a, buffer);
}


void DataLogger__getAverageValues (uint8_t measureNumber, uint8_t *negative_a, uint8_t *t_int_a, uint8_t *t_frac_a)
{
	uint16_t it, storedValues;
	int64_t t_int_tmp = 0;
	int64_t t_frac_tmp = 0;
	uint8_t negative, t_int, t_frac;

	if (measureNumber <= measIndex)
	{
		storedValues = DataLogger__getNumberOfStoredValuesOfMeasure(measureNumber);

		for (it = 0; it < storedValues; it++)
		{
			Temperature__getValuesFromRaw(DataLogger__getStoredValueOfMeasure(measureNumber, it), &negative, &t_int, &t_frac);

			if (negative == TRUE)
			{
				t_int_tmp -= t_int;
			}
			else
			{
				t_int_tmp += t_int;
			}

			t_frac_tmp += t_frac;
		}

		if (t_int_tmp < 0)
		{
			t_int_tmp = t_int_tmp * (-1);
			*negative_a = TRUE;
		}
		else
		{
			*negative_a = FALSE;
		}

		t_frac_tmp = ((t_frac_tmp  + ((t_int_tmp % storedValues) * 100)) / storedValues);
		*t_int_a = (uint8_t)((t_int_tmp / storedValues) + (t_frac_tmp / 100));
		*t_frac_a = (uint8_t)(t_frac_tmp % 100);
	}
	else
	{
		*negative_a = FALSE;
		*t_int_a = 0;
		*t_frac_a = 0;
	}
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
