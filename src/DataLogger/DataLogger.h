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


void DataLogger__eepromInit (void);
void DataLogger__x10 (void);
void DataLogger__startMeasure (uint16_t (*getValue)(), uint8_t (*trigger)());
void DataLogger__stopMeasure (void);
void DataLogger__pauseMeasure (void);
void DataLogger__resumeMeasure (void);
uint16_t DataLogger__getNumberOfStoredValues (void);
uint16_t DataLogger__getStoredValue (uint16_t index);

#endif /* DATALOGGER_H_ */
