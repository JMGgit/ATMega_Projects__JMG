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
#include "Mode_Stats.h"
#include "../Lcd/Lcd.h"


#define	MODE__STANDBY				1
#define MODE__SETUP					2
#define	MODE__SETUP_TIME			3
#define	MODE__SETUP_MEASUREMENT		4
#define	MODE__MEASUREMENT_START		5
#define	MODE__MEASUREMENT			6
#define	MODE__MEASUREMENT_STATS		7
#define MODE__STATS					8


/* public functions */
void Modes__init (void);
void Modes__x10 (void);
void Modes__setMode (uint8_t mode);


#endif /* MODES_H_ */
