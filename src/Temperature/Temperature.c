/*
 * Temperature.c
 *
 *  Created on: 25.05.2014
 *      Author: Jean-Martin George
 */


#include "Temperature.h"


void Temperature__getCurrentValues (uint8_t *t_int, uint8_t *t_frac)
{
#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)
	DS18B20__getCurrentValues(t_int, t_frac);
#endif
}


void Temperature__getCurrentValueString (char *buffer)
{
	uint8_t t_int, t_frac;

	Temperature__getCurrentValues(&t_int, &t_frac);
	itoa(t_int, &buffer[0], 10);
	strcpy(&buffer[2], "'");
	itoa(t_frac, &buffer[3], 10);
}
