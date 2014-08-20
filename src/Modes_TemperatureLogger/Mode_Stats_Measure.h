/*
 * Mode_Stats_Measure.h
 *
 *  Created on: 10.06.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_STATS_MEASURE_H_
#define MODE_STATS_MEASURE_H_


#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Buttons/Buttons.h"
#include "DataLogger/DataLogger.h"
#include "../Clock/Clock.h"
#include "../Temperature/Temperature.h"
#include "../Lcd/Lcd.h"


void Mode_StatsMeasure__x10 (void);

void Mode_StatsMeasure__init (uint8_t measure);
uint8_t Modes__statsMeasureToStats (void);

#endif /* MODE_STATS_MEASURE_H_ */
