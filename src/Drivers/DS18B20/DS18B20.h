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
#include "Main_Types.h"
#include "Main_Config.h"
#include "Drivers_Config.h"
#include "uC.h"


#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)

void DS18B20__init (void);
void DS18B20__x10 (void);
uint8_t DS18B20__getCurrentRawValue (uint16_t *value);

#endif

#endif /* DS18B20_H_ */
