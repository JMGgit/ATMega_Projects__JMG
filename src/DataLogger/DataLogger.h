/*
 * DataLogger.h
 *
 *  Created on: 28.05.2014
 *      Author: Jean-Martin George
 */

#ifndef DATALOGGER_H_
#define DATALOGGER_H_


#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Drivers/Drivers.h"
#include "../Temperature/Temperature.h"
#include "../Modes/Mode_Setup_Measurement.h"


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
} measurementInfos_t;

void DataLogger__eepromInit (void);
void DataLogger__x10 (void);
void DataLogger__startMeasure (uint16_t (*getValue)());
void DataLogger__stopMeasure (void);
uint16_t DataLogger__getNumberOfStoredValues (void);
uint16_t DataLogger__getStoredValue (uint16_t index);
uint16_t DataLogger__getNumberOfStoredValuesOfMeasure (uint8_t measureNumber);
uint16_t DataLogger__getStoredValueOfMeasure (uint8_t measureNumber, uint16_t index);
void DataLogger__getLastValueWithTime (uint16_t *lastMesYear, uint8_t *lastMesMonth, uint8_t *lastMesDate, uint8_t *lastMesHour, uint8_t *lastMesMin, uint8_t *lastMesSec, uint16_t *lastData);
void DataLogger__getValueWithTime (uint8_t measureNumber, uint16_t index, uint16_t *year, uint8_t *mesMonth, uint8_t *mesDate, uint8_t *mesHour, uint8_t *mesMin, uint8_t *mesSec, uint16_t *data);
void DataLogger__getStartTimeOfMeasure (uint8_t measureNumber, uint16_t *mesYear, uint8_t *mesMonth, uint8_t *mesDate, uint8_t *mesHour, uint8_t *mesMin, uint8_t *mesSec);
void DataLogger__getIntervalAndUnitOfMeasure (uint8_t measureNumber, uint8_t *interval, uint8_t *unit);
uint8_t DataLogger__getNumberOfMeasures (void);


#endif /* DATALOGGER_H_ */
