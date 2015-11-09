/*
 * Mode_MeasurementStats.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_MEASUREMENTSTATS_H_
#define MODE_MEASUREMENTSTATS_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Buttons.h"
#include "Lcd.h"


void Mode_MeasurementStats__x10 (void);

void Mode_MeasurementStats__init (void);
uint8_t Modes__measurementStatsToMeasurement (void);


#endif /* MODE_MEASUREMENTSTATS_H_ */
