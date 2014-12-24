/*
 * TWI.c
 *
 *  Created on: 27.03.2013
 *      Author: Jean-Martin George
 */


#include "TWI.h"


void TWI__masterInit (void)
{
	TWSR = 0; /* no prescaler */
	TWBR = 92; /* 100 kHz */
/* 16 Mhz -> 72 */
/* 20 Mhz -> 92 */
}


void TWI__transmitData (uint8_t *data, uint8_t dataLength, uint8_t slaveAddress)
{
	uint8_t idxByte;

	/* send start condition */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	/* wait for TWINT Flag set. This indicates that the START condition has been transmitted */
	while (!(TWCR & (1 << TWINT)))
	{
		;
	}

	/* load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address */
	TWDR = (((slaveAddress) << 1) & 0xFE) | TWI_WRITE_BIT; /* slave address and write bit */
	TWCR = (1 << TWINT) | (1 << TWEN);

	/* wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received. */
	while (!(TWCR & (1 << TWINT)))
	{
		;
	}

	for (idxByte = 0; idxByte < dataLength; idxByte++)
	{
		/* load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data */
		TWDR = data[idxByte];
		TWCR = (1 << TWINT) | (1 << TWEN);

		/* wait for TWINT Flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received. */
		while (!(TWCR & (1 << TWINT)))
		{
			;
		}
	}

	/* transmit STOP condition*/
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while (TWCR & (1 << TWSTO))
	{
		;
	}
}


void TWI__readData (uint8_t *data, uint8_t dataLength, uint8_t slaveAddress)
{
	uint8_t idxByte;

	/* send start condition */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	/* wait for TWINT Flag set. This indicates that the START condition has been transmitted */
	while (!(TWCR & (1 << TWINT)))
	{
		;
	}

	/* load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address */
	TWDR = (((slaveAddress) << 1) & 0xFE) | TWI_READ_BIT; /* slave address and write bit */
	TWCR = (1 << TWINT) | (1 << TWEN);

	/* wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received. */
	while (!(TWCR & (1 << TWINT)))
	{
		;
	}

	/* receive data */
	for (idxByte = 0; idxByte < dataLength; idxByte++)
	{
		if (idxByte < (dataLength - 1))
		{
			/* request more data -> ACK */
			TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
		}
		else
		{
			/* last byte -> no ACK */
			TWCR = (1 << TWINT) | (1 << TWEN);
		}

		while (!(TWCR & (1 << TWINT)))
		{
			;
		}

		data[idxByte] = TWDR;
	}

	/* transmit STOP condition*/
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
