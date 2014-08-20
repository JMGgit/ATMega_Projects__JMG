/*
 * DataLogger.h
 *
 *  Created on: 28.05.2014
 *      Author: Jean-Martin George
 */

#ifndef DATALOGGER_H_
#define DATALOGGER_H_


#include "../../Main/Main_Types.h"
#include "../../Main/Main_Config.h"
#include "../../Drivers/Drivers.h"
#include "../../Temperature/Temperature.h"
#include "../Mode_Setup_Measurement.h"


#define DATA_LOGGER_MEASURES_NB		9

typedef struct
{
	uint16_t yearStart;
	uint8_t monthStart;
	uint8_t dateStart;
	uint8_t hourStart;
	uint8_t minuteStart;
	uint8_t secondStart;
	uint8_t interval;
	uint8_t unit;
	uint16_t min;
	uint16_t max;
} measurementInfos_t;

void DataLogger__eepromInit (void);
void DataLogger__x10 (void);
void DataLogger__startMeasure (uint16_t (*getValue)());
void DataLogger__stopMeasure (void);
uint16_t DataLogger__getNumberOfStoredValues (void);
uint16_t DataLogger__getStoredValue (uint16_t index);
uint16_t DataLogger__getNumberOfStoredValuesOfMeasure (uint8_t measureNumber);
uint16_t DataLogger__getFreeMeasurementPoints (void);
uint16_t DataLogger__getStoredValueOfMeasure (uint8_t measureNumber, uint16_t index);
void DataLogger__getLastValueWithTime (uint16_t *lastMeasYear, uint8_t *lastMeasMonth, uint8_t *lastMeasDate, uint8_t *lastMeasHour, uint8_t *lastMeasMin, uint8_t *lastMeasSec, uint16_t *lastData);
void DataLogger__getValueWithTime (uint8_t measureNumber, uint16_t index, uint16_t *year, uint8_t *measMonth, uint8_t *measDate, uint8_t *measHour, uint8_t *measMin, uint8_t *measSec, uint16_t *data);
void DataLogger__getStartTimeOfMeasure (uint8_t measureNumber, uint16_t *measYear, uint8_t *measMonth, uint8_t *measDate, uint8_t *measHour, uint8_t *measMin, uint8_t *measSec);
void DataLogger__getIntervalAndUnitOfMeasure (uint8_t measureNumber, uint8_t *interval, uint8_t *unit);
uint8_t DataLogger__getNumberOfMeasures (void);
void DataLogger__getMinValueString (uint8_t measureNumber, char *buffer);
void DataLogger__getMinValues (uint8_t measureNumber, uint8_t *negative, uint8_t *t_int, uint8_t *t_frac);
void DataLogger__getMaxValueString (uint8_t measureNumber, char *buffer);
void DataLogger__getMaxValues (uint8_t measureNumber, uint8_t *negative, uint8_t *t_int, uint8_t *t_frac);
void DataLogger__getAverageValueString (uint8_t measureNumber, char *buffer);
void DataLogger__getAverageValues (uint8_t measureNumber, uint8_t *negative_a, uint8_t *t_int_a, uint8_t *t_frac_a);
void DataLogger__clearMeasure (uint8_t measureNumber); /* TODO: NOT TESTED!! */
void DataLogger__clearAll (void);


#endif /* DATALOGGER_H_ */
