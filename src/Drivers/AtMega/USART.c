/*
 * USART.c
 *
 *  Created on: 27.03.2013
 *      Author: Jean-Martin George
 */


#include "USART.h"

#define BAUD 57600UL
#define UBRR_VAL ((F_CPU + BAUD * 4) / (BAUD * 8) - 1)
#define BAUD_REAL (F_CPU / (8 * (UBRR_VAL + 1)))
#define BAUD_ERROR ((BAUD_REAL * 1000) / BAUD)

#if ((BAUD_ERROR < 990) || (BAUD_ERROR > 1010))
  #error too high USART error rate!
#endif


#if (USART_DATA_LENGTH_READ_MAX != 0)
static volatile uint8_t USART_data[USART_DATA_LENGTH_READ_MAX];
static volatile uint8_t USART_idxData = 0;
static volatile uint8_t USART_currentDataSize = 0;
#endif

void USART__init (void)
{
	/* baud rate */
	UCSR0A = (1 << U2X0);
	UBRR0L = UBRR_VAL & 0xFF;
	UBRR0H = (UBRR_VAL >> 8) & 0xFF;

	/* enable receiver with interrupt and transmitter */
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);

	/* asynchronous transmission, no parity, 1 stop bit, 8 data bits */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


#if (USART_DATA_LENGTH_READ_MAX != 0)
/* interrupt receive routine */
ISR(USART0_RX_vect)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		USART_data[USART_idxData] = UDR0;

		if (USART_idxData == 0)
		{
			/* read data size */
			USART_currentDataSize = USART_data[0] & 0x0F;
		}

		USART_idxData++;

		if (USART_idxData >= USART_currentDataSize)
		{
			USART_idxData = 0;
			USART_currentDataSize = 0;
		}
	}
}


uint8_t USART__readData (uint8_t *data, uint8_t dataLength, uint8_t requester)
{
	uint8_t idxByte, dataSize;
	uint8_t retVal = E_NOT_OK;

	/* TODO: add defines */
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if ((requester & (USART_data[0] & 0xF0)) == requester)
		{
			dataSize = USART_data[0] & 0x0F;

			if (dataLength == dataSize)
			{
				for (idxByte = 0; idxByte < dataLength; idxByte++)
				{
					data[idxByte] = USART_data[idxByte];
					USART_data[idxByte] = 0;
				}

				retVal = E_OK;
			}
		}
	}
	return (retVal);
}

#endif


void USART__transmitData (uint8_t *data, uint8_t dataLength)
{
	uint8_t idxByte;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (idxByte = 0; idxByte < dataLength; idxByte++)
		{
			/* wait for empty transmit buffer */
			while (!(UCSR0A & (1 << UDRE0)))
			{
				;
			}

			UDR0 = data[idxByte];
		}
	}
}
