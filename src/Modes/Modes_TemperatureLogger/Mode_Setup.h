/*
 * Mode_Setup.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_SETUP_H_
#define MODE_SETUP_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Buttons.h"
#include "Lcd.h"
#include "Clock.h"


void Mode_Setup__init (void);
void Mode_Setup__x10 (void);

uint8_t Modes__setupToStandby (void);
uint8_t Modes__setupToSetupTime (void);
uint8_t Modes__setupToSetupMeasurement (void);


#endif /* MODE_SETUP_H_ */
