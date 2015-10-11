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

#if (SPI_SPEED == SPI_CLK_DIV_4)
	/* enable SPI, master, clock rate fck/4 */
	SPCR = (1 << SPE) | (1 << MSTR);
	SPSR = 0;
#elif (SPI_SPEED == SPI_CLK_DIV_16)
	/* enable SPI, master, clock rate fck/16 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	SPSR = 0;
#endif
}


void SPI__transmitData (uint8_t *data, uint16_t dataLength)
{
	uint8_t l_data = *data;

	SPDR = l_data;
	data++;
	dataLength--;

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
}
