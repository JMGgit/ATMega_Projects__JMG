/*
 * Mode_Standby.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_STANDBY_H_
#define MODE_STANDBY_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Buttons/Buttons.h"
#include "../Lcd/Lcd.h"
#include "../Clock/Clock.h"


void Mode_Standby__init (void);
void Mode_Standby__x10 (void);

uint8_t Modes__standbyToSetup (void);
uint8_t Modes__standbyToMeasurementStart (void);


#endif /* MODE_STANDBY_H_ */
