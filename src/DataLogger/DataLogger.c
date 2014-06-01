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
#define DL_MODE__PAUSE	3

static uint16_t dataLog[MAX_MEASUREMENT_POINTS];

static uint8_t mode;
static uint16_t (*getCurrentData)();
static uint8_t (*getTrigger)();
static uint16_t dataLogIt;


void DataLogger__eepromInit (void)
{
	mode = DL_MODE__IDLE;
	/* TODO: save in EEPROM */
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
}


void DataLogger__startMeasure (uint16_t (*getValue)(), uint8_t (*trigger)())
{
	if ((mode == DL_MODE__IDLE) || (mode == DL_MODE__PAUSE))
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
		mode = DL_MODE__PAUSE;
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
