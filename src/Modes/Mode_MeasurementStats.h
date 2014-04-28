/*
 * Mode_MeasurementStats.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_MEASUREMENTSTATS_H_
#define MODE_MEASUREMENTSTATS_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Buttons/Buttons.h"
#include "../Lcd/Lcd.h"


void Mode_MeasurementStats__x10 (void);

void Mode_MeasurementStats__init (void);
uint8_t Modes__measurementStatsToMeasurement (void);


#endif /* MODE_MEASUREMENTSTATS_H_ */
