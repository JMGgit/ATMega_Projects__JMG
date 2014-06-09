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


#define DATA_LOGGER_MEASURES_NB		10

typedef struct
{
	uint8_t monthStart;
	uint8_t dateStart;
	uint8_t hourStart;
	uint8_t minuteStart;
	uint8_t secondStart;
} measurementInfos_t;

void DataLogger__eepromInit (void);
void DataLogger__x10 (void);
void DataLogger__startMeasure (uint16_t (*getValue)());
void DataLogger__stopMeasure (void);
uint16_t DataLogger__getNumberOfStoredValues (void);
uint16_t DataLogger__getStoredValue (uint16_t index);
uint16_t DataLogger__getNumberOfStoredValuesOfMeasure (uint8_t measureNumber);
uint16_t DataLogger__getStoredValueOfMeasure (uint8_t measureNumber, uint16_t index);
void DataLogger__getLastValueWithTime (uint8_t *lastMesMonth, uint8_t *lastMesDate, uint8_t *lastMesHour, uint8_t *lastMesMin, uint8_t *lastMesSec, uint16_t *lastData);


#endif /* DATALOGGER_H_ */
