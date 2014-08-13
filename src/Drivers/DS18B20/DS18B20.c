/*
 * DS18B20.c
 *
 *  Created on: 23.03.2014
 *      Author: Jean-Martin George
 */

#include "DS18B20.h"


#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)

#define CMD_ROM__READ		0x33
#define CMD_ROM__MATCH		0x55
#define CMD_ROM__SKIP		0xCC
#define CMD_ROM__SEARCH		0xF0
#define CMD_ROM__ALARM		0xEC

#define CMD_MEM__WRITE		0x4E
#define CMD_MEM__READ		0xBE
#define CMD_MEM__COPY		0x48
#define CMD_MEM__CONVERT	0x44
#define CMD_MEM__RECALL		0xB8
#define CMD_MEM__POWER		0xB4

#define CONFIG__RES_9		0x00
#define CONFIG__RES_10		0x01
#define CONFIG__RES_11		0x02
#define CONFIG__RES_12		0x03

#define READ_BUFFER_SIZE	9

static uint16_t currentTempValue;


static void DS18B20__write (uint8_t data)
{
	uint8_t bitIt;

	for (bitIt = 0; bitIt < 8; bitIt++)
	{
		/* generate write slot */
		_delay_us(10);
		setOutput(DS18B20_DATA_DDR, DS18B20_DATA_PIN);
		setLow(DS18B20_DATA_PORT, DS18B20_DATA_PIN);

		if ((data & (1 << bitIt)) > 0)
		{
			_delay_us(10);
			setHigh(DS18B20_DATA_PORT, DS18B20_DATA_PIN);
		}
		else
		{
			setLow(DS18B20_DATA_PORT, DS18B20_DATA_PIN);
		}

		_delay_us(50);
		setInput(DS18B20_DATA_DDR, DS18B20_DATA_PIN);
	}
}


static void DS18B20__sendCommand (uint8_t command)
{
	/* init: reset and presence pulses */
	setOutput(DS18B20_DATA_DDR, DS18B20_DATA_PIN);
	setHigh(DS18B20_DATA_PORT, DS18B20_DATA_PIN);
	setLow(DS18B20_DATA_PORT, DS18B20_DATA_PIN);
	_delay_us(500);
	setInput(DS18B20_DATA_DDR, DS18B20_DATA_PIN);
	_delay_us(100);

	/* check presence pulse */
	if (isLow(DS18B20_DATA_PINR, DS18B20_DATA_PIN))
	{
		_delay_us(200);
		DS18B20__write(CMD_ROM__SKIP);
		DS18B20__write(command);
	}
}



static void DS18B20__read (uint8_t *data, uint8_t length)
{
	uint8_t byteIt, bitIt;

	for (byteIt = 0; byteIt < length; byteIt++)
	{
		data[byteIt] = 0;

		for (bitIt = 0; bitIt < 8; bitIt++)
		{
			/* generate read slot */
			_delay_us(10);
			setOutput(DS18B20_DATA_DDR, DS18B20_DATA_PIN);
			setLow(DS18B20_DATA_PORT, DS18B20_DATA_PIN);
			_delay_us(2);
			setInput(DS18B20_DATA_DDR, DS18B20_DATA_PIN);
			_delay_us(12);

			if (isHigh(DS18B20_DATA_PINR, DS18B20_DATA_PIN))
			{
				data[byteIt] |= (1 << bitIt);
			}

			_delay_us(50);
		}
	}
}


void DS18B20__init (void)
{
	uint8_t dataBuffer[READ_BUFFER_SIZE];

	DS18B20__sendCommand(CMD_MEM__CONVERT);

	DS18B20__sendCommand(CMD_MEM__READ);
	DS18B20__read(dataBuffer, READ_BUFFER_SIZE);

	if (	(dataBuffer[2] != DS18B20_ALARM_THIGH)
		||	(dataBuffer[3] != DS18B20_ALARM_TLOW)
		||	(dataBuffer[4] != CONFIG__RES_12)
		)
	{
		DS18B20__sendCommand(CMD_MEM__WRITE);
		DS18B20__sendCommand(DS18B20_ALARM_THIGH);
		DS18B20__sendCommand(DS18B20_ALARM_TLOW);
		DS18B20__sendCommand(CONFIG__RES_12);
		DS18B20__sendCommand(CMD_MEM__COPY);
	}

	/* initial conversion */
	DS18B20__sendCommand(CMD_MEM__CONVERT);
}


void DS18B20__x10 (void)
{
	uint8_t dataBuffer[READ_BUFFER_SIZE];

	setInput(DS18B20_DATA_DDR, DS18B20_DATA_PIN);

	/* bus idle */
	if (isHigh(DS18B20_DATA_PINR, DS18B20_DATA_PIN))
	{
		/* read value */
		DS18B20__sendCommand(CMD_MEM__READ);
		DS18B20__read(dataBuffer, READ_BUFFER_SIZE);

		/* update last temp */
		currentTempValue = dataBuffer[0];
		currentTempValue |= (dataBuffer[1] << 8);

		/* trigger new conversion */
		DS18B20__sendCommand(CMD_MEM__CONVERT);
	}
}


uint16_t DS18B20__getCurrentRawValue (void)
{
	return currentTempValue;
}

#endif
