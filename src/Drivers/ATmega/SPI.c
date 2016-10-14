/*
 * SPI.c
 *
 *  Created on: 27.03.2013
 *      Author: Jean-Martin George
 */


#include "SPI.h"


#if (SPI_SPEED != SPI_DISABLED)

void SPI__masterInit (void)
{
	setOutput(SPI_DDR, SPI_PIN_SS);
	setOutput(SPI_DDR, SPI_PIN_MOSI);
	setInput(SPI_DDR, SPI_PIN_MISO);
	setOutput(SPI_DDR, SPI_PIN_CLK);
	setHigh(SPI_PORT, SPI_PIN_SS);
	setLow(SPI_PORT, SPI_PIN_MOSI);
	setLow(SPI_PORT, SPI_PIN_CLK);

#if (SPI_SPEED == SPI_CLK_DIV_8)
	/* enable SPI, master, clock rate fck/8 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	SPSR = 1 << SPI2X;
#elif (SPI_SPEED == SPI_CLK_DIV_16)
	/* enable SPI, master, clock rate fck/16 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
#elif (SPI_SPEED == SPI_CLK_DIV_2)
	SPCR = (1 << SPE) | (1 << MSTR);
	SPSR = 1 << SPI2X;
#else
	#error : SPI speed not defined!
#endif
}


void SPI__transmitData (uint8_t *data, uint16_t dataLength)
{
	uint8_t l_data = *data;

	SPDR = l_data;
	data++;
	dataLength--;

	Debug__setWhileState(WHILE_STATE_SPI_BEFORE);
 	do
 	{
 		l_data = *data;
 		data++;

 		/* wait for transmission complete */
		while (!(SPSR & (1<<SPIF)))
		{
			;
		}

		SPDR = l_data;
 	}
 	while (--dataLength);

 	/* wait for transmission complete */
	while (!(SPSR & (1<<SPIF)))
	{
		;
	}
	Debug__setWhileState(WHILE_STATE_SPI_AFTER);
}


void SPI__slaveSelect (volatile uint8_t *slaveDdr, volatile uint8_t *slavePort, uint8_t slavePin, uint8_t enableSlave)
{
	setOutput(*slaveDdr, slavePin);

	if (enableSlave)
	{
		setLow(*slavePort, slavePin);
	}
	else
	{
		setHigh(*slavePort, slavePin);
	}
}

#endif
