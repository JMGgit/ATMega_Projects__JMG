/*
 * Modes.h
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MODES_TEMPERATURELOGGER_H_
#define MODES_TEMPERATURELOGGER_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Mode_Measurement.h"
#include "Mode_MeasurementStart.h"
#include "Mode_MeasurementStats.h"
#include "Mode_Setup.h"
#include "Mode_Setup_Time.h"
#include "Mode_Setup_Measurement.h"
#include "Mode_Standby.h"
#include "Mode_Stats.h"
#include "Mode_Stats_Measure.h"
#include "Mode_Transfer.h"
#include "Lcd.h"


typedef enum
{
	MODE__STANDBY = 0,
	MODE__SETUP,
	MODE__SETUP_TIME,
	MODE__SETUP_MEASUREMENT,
	MODE__MEASUREMENT_START,
	MODE__MEASUREMENT,
	MODE__MEASUREMENT_STATS,
	MODE__STATS,
	MODE__STATS_MESURE,
	MODE__TRANSFER
} Mode_t;


#endif /* MODES_TEMPERATURELOGGER_H_ */
