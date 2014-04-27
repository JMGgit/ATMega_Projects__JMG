/*
 * Mode_Setup_Time.h
 *
 *  Created on: 15.04.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_SETUP_TIME_H_
#define MODE_SETUP_TIME_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Buttons/Buttons.h"
#include "../Lcd/Lcd.h"
#include "../Clock/Clock.h"


void Mode_SetupTime__init (void);
void Mode_SetupTime__x10 (void);

uint8_t Modes__setupTimeToSetup (void);


#endif /* MODE_SETUP_TIME_H_ */
