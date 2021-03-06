/*
 * Mode_Stats.h
 *
 *  Created on: 09.06.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_STATS_H_
#define MODE_STATS_H_


#include "Main_Types.h"
#include "Main_Config.h"
#include "Buttons.h"
#include "Lcd.h"
#include "Clock.h"
#include "DataLogger.h"


void Mode_Stats__init (void);
void Mode_Stats__x10 (void);
uint8_t Modes__statsToStandby (void);
uint8_t Modes__statsToStatsMeasure (uint8_t *measure);

#endif /* MODE_STATS_H_ */
