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

	/* enable SPI, master, clock rate fck/16 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	SPSR = 0;
}


void SPI__transmitData (uint8_t *data, uint8_t dataLength)
{
	uint8_t idxByte;

	for (idxByte = 0; idxByte < dataLength; idxByte++)
	{
		/* copy data to register */
		SPDR = *data;

		/* wait for transmission complete */
		while (!(SPSR & (1 << SPIF)))
		{
			;
		}
	}
}
