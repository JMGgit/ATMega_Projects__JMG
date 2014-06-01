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


uint16_t DS18B20__getCurrentRawValue (void)
{
	return 0x191;
}

#endif
