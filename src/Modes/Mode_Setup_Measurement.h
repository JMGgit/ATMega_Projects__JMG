/*
 * Mode_Setup_Measurement.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_SETUP_MEASUREMENT_H_
#define MODE_SETUP_MEASUREMENT_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Buttons/Buttons.h"
#include "../Lcd/Lcd.h"
#include "../Clock/Clock.h"


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
