/*
 * Mode_Setup_Time.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_SETUP_TIME_H_
#define MODE_SETUP_TIME_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Buttons.h"
#include "Lcd.h"
#include "Clock.h"


void Mode_SetupTime__init (void);
void Mode_SetupTime__x10 (void);

uint8_t Modes__setupTimeToSetup (void);


#endif /* MODE_SETUP_TIME_H_ */
