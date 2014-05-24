/*
 * DS18B20.h
 *
 *  Created on: 23.03.2014
 *      Author: Jean-Martin George
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "../../Main/Main_Types.h"
#include "../../Main/Main_Config.h"
#include "../Drivers_Config.h"

#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)

void DS18B20__init (void);
void DS18B20__getCurrentValues (uint8_t *t_int, uint8_t *t_frac);

#endif

#endif /* DS18B20_H_ */
