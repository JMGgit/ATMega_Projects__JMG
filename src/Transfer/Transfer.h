/*
 * Transfer.h
 *
 *  Created on: 10.06.2014
 *      Author: Jean-Martin George
 */

#ifndef TRANSFER_H_
#define TRANSFER_H_


#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Drivers/Drivers.h"
#include "../DataLogger/DataLogger.h"


void Transfer__init(void);
void Transfer__start (void);
void Transfer__x10 (void);
uint8_t Transfer__getProgress (void);


#endif /* TRANSFER_H_ */