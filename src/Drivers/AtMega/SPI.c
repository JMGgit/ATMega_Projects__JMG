/*
 * SPI.c
 *
 *  Created on: 27.03.2013
 *      Author: Jean-Martin George
 */


#include "SPI.h"


void SPI__masterInit (void)
{
	setOutput(SPI_DDR, SPI_PIN_SS);
	setOutput(SPI_DDR, SPI_PIN_MOSI);
	setInput(SPI_DDR, SPI_PIN_MISO);
	setOutput(SPI_DDR, SPI_PIN_CLK);
	setHigh(SPI_PORT, SPI_PIN_SS);
	setLow(SPI_PORT, SPI_PIN_MOSI);
	setLow(SPI_PORT, SPI_PIN_CLK);

	/* enable SPI, master, clock rate fck/8 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	SPSR = 1 << SPI2X;
}


void SPI__transmitData (uint8_t *data, uint16_t dataLength)
{
	uint8_t buffer;

	SPDR = *data;

	/* wait for transmission complete */
	while (!(SPSR & (1 << SPIF)))
	{
		;
	}

	dataLength--;

	while (dataLength > 0)
	{
		buffer = *data;
		data++;

		/* wait for transmission complete */
		while (!(SPSR & (1 << SPIF)))
		{
			;
		}

		/* copy data to register */
		SPDR = buffer;

		dataLength--;
	}
}
