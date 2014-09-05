/*
 * Temperature.c
 *
 *  Created on: 25.05.2014
 *      Author: Jean-Martin George
 */


#include "Temperature.h"
#include "../Clock/Clock.h" /* TEST */

uint8_t Temperature__getCurrentRawValue (uint16_t *value)
{
#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)
	return DS18B20__getCurrentRawValue(value);
#elif (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_TEST)
	*value = 0;

	if ((Clock__getSeconds() / 50) > 0)
	{
		*value |= 0x08;
	}

	if (((Clock__getSeconds() % 50) / 25) > 0)
	{
		*value |= 0x04;
	}

	if ((((Clock__getSeconds() % 50) % 25) / 12) > 0)
	{
		*value |= 0x02;
	}

	if ((((((Clock__getSeconds() % 50) % 25) % 12) / 6) > 0) || (((Clock__getSeconds() % 50) % 25) / 12) == 2)
	{
		*value |= 0x01;
	}

	*value |= Clock__getMinutes() << 4;

	return E_OK;
#else
	*value = 0;

	return E_OK;
#endif
}


void Temperature__getCurrentValues (uint8_t *negative, uint8_t *t_int, uint8_t *t_frac)
{
	uint16_t rawValue;

	if (Temperature__getCurrentRawValue(&rawValue) == E_OK)
	{
		Temperature__getValuesFromRaw(rawValue, negative, t_int, t_frac);
	}
	else
	{
		negative = FALSE;
		t_int = 0;
		t_frac = 0;
	}
}


void Temperature__getValuesFromRaw (uint16_t rawValue, uint8_t *negative,  uint8_t *t_int, uint8_t *t_frac)
{
#if ((TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20) || (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_TEST))
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
#else
	*negative = 0;
	*t_int = 0;
	*t_frac = 0;
#endif
}


uint16_t Temperature__getMinRawValue (uint16_t rawValue1, uint16_t rawValue2)
{
	uint8_t negative1, t_int1, t_frac1, negative2, t_int2, t_frac2;
	uint16_t retVal;

	Temperature__getValuesFromRaw(rawValue1, &negative1, &t_int1, &t_frac1);
	Temperature__getValuesFromRaw(rawValue2, &negative2, &t_int2, &t_frac2);

	if (negative2 == TRUE)
	{
		if (negative1 == FALSE)
		{
			retVal = rawValue2;
		}
		else
		{
			if (t_int2 > t_int1)
			{
				retVal = rawValue2;
			}
			else
			{
				if (t_int2 == t_int1)
				{
					if (t_frac2 > t_frac1)
					{
						retVal = rawValue2;
					}
					else
					{
						retVal = rawValue1;
					}
				}
				else
				{
					retVal = rawValue1;
				}
			}
		}
	}
	else
	{
		if (negative1 == TRUE)
		{
			retVal = rawValue1;
		}
		else
		{
			if (t_int1 > t_int2)
			{
				retVal = rawValue2;
			}
			else
			{
				if (t_int1 == t_int2)
				{
					if (t_frac1 > t_frac2)
					{
						retVal = rawValue2;
					}
					else
					{
						retVal = rawValue1;
					}
				}
				else
				{
					retVal = rawValue1;
				}
			}
		}
	}

	return retVal;
}


uint16_t Temperature__getMaxRawValue (uint16_t rawValue1, uint16_t rawValue2)
{
	uint8_t negative1, t_int1, t_frac1, negative2, t_int2, t_frac2;
	uint16_t retVal;

	Temperature__getValuesFromRaw(rawValue1, &negative1, &t_int1, &t_frac1);
	Temperature__getValuesFromRaw(rawValue2, &negative2, &t_int2, &t_frac2);

	if (negative2 == TRUE)
	{
		if (negative1 == FALSE)
		{
			retVal = rawValue1;
		}
		else
		{
			if (t_int2 > t_int1)
			{
				retVal = rawValue1;
			}
			else
			{
				if (t_int2 == t_int1)
				{
					if (t_frac2 > t_frac1)
					{
						retVal = rawValue1;
					}
					else
					{
						retVal = rawValue2;
					}
				}
				else
				{
					retVal = rawValue2;
				}
			}
		}
	}
	else
	{
		if (negative1 == TRUE)
		{
			retVal = rawValue2;
		}
		else
		{
			if (t_int1 > t_int2)
			{
				retVal = rawValue1;
			}
			else
			{
				if (t_int2 == t_int1)
				{
					if (t_frac1 > t_frac2)
					{
						retVal = rawValue1;
					}
					else
					{
						retVal = rawValue2;
					}
				}
				else
				{
					retVal = rawValue2;
				}
			}
		}
	}

	return retVal;
}


void Temperature__getCurrentValueString (char *buffer)
{
	uint16_t rawValue;

	if (Temperature__getCurrentRawValue(&rawValue) == E_OK)
	{
		Temperature__getValueStringFromRaw(rawValue, buffer);
	}
	else
	{
		Temperature__getValueStringFromRaw(0, buffer);
	}
}


void Temperature__getValueString (uint8_t negative, uint8_t t_int, uint8_t t_frac, char *buffer)
{
	uint8_t idxBuffer = 0;

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


void Temperature__getValueStringFromRaw (uint16_t rawValue, char *buffer)
{
	uint8_t negative, t_int, t_frac;

	Temperature__getValuesFromRaw(rawValue, &negative, &t_int, &t_frac);
	Temperature__getValueString(negative, t_int, t_frac, buffer);
}
