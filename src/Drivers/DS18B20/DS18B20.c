/*
 * DS18B20.c
 *
 *  Created on: 23.03.2014
 *      Author: Jean-Martin George
 */

#include "DS18B20.h"


#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)

void DS18B20__init (void)
{

}


void DS18B20__getCurrentValues (uint8_t *t_int, uint8_t *t_frac)
{
	*t_int = 27;
	*t_frac = 13;
}

#endif
