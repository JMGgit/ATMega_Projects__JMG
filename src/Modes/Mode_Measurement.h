/*
 * Mode_Measurement.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_MEASUREMENT_H_
#define MODE_MEASUREMENT_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Buttons/Buttons.h"
#include "../Lcd/Lcd.h"
#include "../Clock/Clock.h"
#include "../Temperature/Temperature.h"
#include "../DataLogger/DataLogger.h"


void Mode_Measurement__eepromInit (void);
void Mode_Measurement__init (void);
void Mode_Measurement__x10 (void);
uint8_t Modes__measurementToMeasurementStats (void);
uint8_t Modes__measurementToStandby (void);


#endif /* MODE_MEASUREMENT_H_ */
