/*
 * Mode_Setup.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_SETUP_H_
#define MODE_SETUP_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Buttons/Buttons.h"
#include "../Lcd/Lcd.h"
#include "../Clock/Clock.h"


void Mode_Setup__init (void);
void Mode_Setup__x10 (void);

uint8_t Modes__setupToStandby (void);
uint8_t Modes__setupToSetupTime (void);
uint8_t Modes__setupToSetupMeasurement (void);


#endif /* MODE_SETUP_H_ */
