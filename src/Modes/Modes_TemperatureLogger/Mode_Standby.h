/*
 * Mode_Standby.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_STANDBY_H_
#define MODE_STANDBY_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Buttons.h"
#include "Lcd.h"
#include "Clock.h"
#include "Temperature.h"


void Mode_Standby__init (void);
void Mode_Standby__x10 (void);

uint8_t Modes__standbyToSetup (void);
uint8_t Modes__standbyToMeasurementStart (void);
uint8_t Modes__standbyToStats (void);
uint8_t Modes__standbyToTransfer (void);


#endif /* MODE_STANDBY_H_ */
