/*
 * Mode_MeasurementStart.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_MEASUREMENTSTART_H_
#define MODE_MEASUREMENTSTART_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Buttons/Buttons.h"
#include "../Lcd/Lcd.h"
#include "../Clock/Clock.h"


void Mode_MeasurementStart__init (void);
void Mode_MeasurementStart__x10 (void);

uint8_t Modes__measurementStartToMeasurement (void);
uint8_t Modes__measurementStartToStandby (void);


#endif /* MODE_MEASUREMENTSTART_H_ */
