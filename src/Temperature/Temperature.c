/*
 * Temperature.c
 *
 *  Created on: 25.05.2014
 *      Author: Jean-Martin George
 */


#include "Temperature.h"


uint16_t Temperature__getCurrentRawValue (void)
{
#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)
	return DS18B20__getCurrentRawValue();
#else
	return 0;
#endif
}


void Temperature__getCurrentValues (uint8_t *negative, uint8_t *t_int, uint8_t *t_frac)
{
#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)
	Temperature__getValuesFromRaw(Temperature__getCurrentRawValue(), negative, t_int, t_frac);
#endif
}


void Temperature__getValuesFromRaw (uint16_t rawValue, uint8_t *negative,  uint8_t *t_int, uint8_t *t_frac)
{
#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)
	if ((rawValue & 0xF800) > 0)
	{
		*negative = TRUE;
		rawValue = 0xFFFF + 1 - rawValue;
	}
	else
	{
		*negative = FALSE;
	}

	*t_int = (rawValue & 0x7F0) >> 4;
	*t_frac = ((rawValue & 0x01) == 0x01) * 6 + ((rawValue & 0x02) == 0x02) * 12 + ((rawValue & 0x04) == 0x04) * 25 + ((rawValue & 0x08) == 0x08) * 50;
#endif
}


void Temperature__getCurrentValueString (char *buffer)
{
	Temperature__getValueStringFromRaw(Temperature__getCurrentRawValue(), buffer);
}


void Temperature__getValueStringFromRaw (uint16_t rawValue, char *buffer)
{
	uint8_t negative, t_int, t_frac;
	uint8_t idxBuffer = 0;

	Temperature__getValuesFromRaw(rawValue, &negative, &t_int, &t_frac);

	if (negative > 0)
	{
		buffer[idxBuffer++] = '-';
	}
	else
	{
		buffer[idxBuffer++] = ' ';
	}

	if (t_int >= 10)
	{
		itoa(t_int, &buffer[idxBuffer++], 10);
		idxBuffer++;

		if (t_int >= 100)
		{
			idxBuffer++;
		}
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(t_int, &buffer[idxBuffer++], 10);
	}

	strcpy(&buffer[idxBuffer++], "'");

	if (t_frac >= 10)
	{
		itoa(t_frac, &buffer[idxBuffer++], 10);

		if (t_int < 100)
		{
			idxBuffer++;
		}
	}
	else
	{
		buffer[idxBuffer++] = '0';

		if (t_int < 100)
		{
			itoa(t_frac, &buffer[idxBuffer++], 10);
		}
	}
}
