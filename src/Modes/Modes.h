/*
 * Modes.h
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MODES_H_
#define MODES_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "Mode_Measurement.h"
#include "Mode_MeasurementStart.h"
#include "Mode_MeasurementStats.h"
#include "Mode_Setup.h"
#include "Mode_Setup_Time.h"
#include "Mode_Setup_Measurement.h"
#include "Mode_Standby.h"


typedef enum
{
	MODE__STANDBY = 0,
	MODE__SETUP,
	MODE__SETUP_TIME,
	MODE__SETUP_MEASUREMENT,
	MODE__MEASUREMENT_START,
	MODE__MEASUREMENT,
	MODE__MEASUREMENT_STATS,
} Mode_t;


/* public functions */
void Modes__init (void);
void Modes__x10 (void);
void Modes__setMode (uint8_t mode);
void Modes__eepromStorage (void);


#endif /* MODES_H_ */
