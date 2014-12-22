/*
 * Mode_Setup_Measurement.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_SETUP_MEASUREMENT_H_
#define MODE_SETUP_MEASUREMENT_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Buttons.h"
#include "Lcd.h"
#include "Clock.h"
#include "DataLogger.h"


#define MEASUREMENT_UNIT_SECOND		1
#define MEASUREMENT_UNIT_MINUTE		2
#define MEASUREMENT_UNIT_HOUR		3
#define MEASUREMENT_UNIT_DAY		4


void Mode_SetupMeasurement__init (void);
void Mode_SetupMeasurement__eepromInit(void);
void Mode_SetupMeasurement__x10 (void);

uint8_t Mode_SetupMeasurement__getInterval (void);
uint8_t Mode_SetupMeasurement__getUnit (void);
void Mode_SetupMeasurement__getIntervalString (char* buffer);
void Mode_SetupMeasurement__getUnitString (char* buffer);
void Mode_SetupMeasurement__getMeasurementTimeString (char* buffer);

uint8_t Modes__setupMeasurementToSetup (void);

#endif /* MODE_SETUP_MEASUREMENT_H_ */
