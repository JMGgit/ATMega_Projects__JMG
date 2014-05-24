/*
 * Temperature.h
 *
 *  Created on: 25.05.2014
 *      Author: Jean-Martin George
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Drivers/Drivers.h"


void Temperature__getCurrentValues (uint8_t *t_int, uint8_t *t_frac);
void Temperature__getCurrentValueString (char *buffer);



#endif /* TEMPERATURE_H_ */
