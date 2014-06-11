/*
 * Mode_Transfer.h
 *
 *  Created on: 10.06.2014
 *      Author: Jean-Martin George
 */

#ifndef MODE_TRANSFER_H_
#define MODE_TRANSFER_H_


#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Buttons/Buttons.h"
#include "../Lcd/Lcd.h"
#include "../Transfer/Transfer.h"


void Mode_Transfer__init (void);
void Mode_Transfer__x10 (void);

uint8_t Modes__transferToStandby (void);


#endif /* MODE_TRANSFER_H_ */
