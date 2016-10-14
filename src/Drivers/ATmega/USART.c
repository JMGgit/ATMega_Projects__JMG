/*
 * USART.c
 *
 *  Created on: 27.03.2013
 *      Author: Jean-Martin George
 */


#include "USART.h"


#if (USART_DATA_LENGTH_READ_MAX != 0)

#define BAUD 250000UL
#define UBRR_VAL ((F_CPU + BAUD * 4) / (BAUD * 8) - 1)
#define BAUD_REAL (F_CPU / (8 * (UBRR_VAL + 1)))
#define BAUD_ERROR ((BAUD_REAL * 1000) / BAUD)

#if ((BAUD_ERROR < 990) || (BAUD_ERROR > 1010))
  #error too high USART error rate!
#endif

static volatile uint8_t USART0_data[USART_DATA_LENGTH_READ_MAX];
static volatile uint8_t USART0_idxData = 0;
static volatile uint8_t USART0_currentDataSize = 0;


void USART__init (void)
{
	/*** USART 0 ***/

	/* baud rate */
	UCSR0A = (1 << U2X0);
	UBRR0L = UBRR_VAL & 0xFF;
	UBRR0H = (UBRR_VAL >> 8) & 0xFF;

	/* enable receiver with interrupt and transmitter */
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);

	/* asynchronous transmission, no parity, 1 stop bit, 8 data bits */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	/*** USART 1 ***/

#ifdef __AVR_ATmega644P__
	/* baud rate */
	UCSR1A = (1 << U2X1);
	UBRR1L = UBRR_VAL & 0xFF;
	UBRR1H = (UBRR_VAL >> 8) & 0xFF;

	/* enable receiver with interrupt and transmitter */
	UCSR1B = (1 << RXCIE1) | (1 << RXEN1) | (1 << TXEN1);

	/* asynchronous transmission, no parity, 1 stop bit, 8 data bits */
	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
#endif
}


ISR(USART0_RX_vect)
{
	USART0_data[USART0_idxData] = UDR0;

	if (USART0_idxData == 0)
	{
		/* read data size */
		USART0_currentDataSize = USART0_data[0] & 0x0F;
	}

	USART0_idxData++;

	if (USART0_idxData >= USART0_currentDataSize)
	{
		USART0_idxData = 0;
		USART0_currentDataSize = 0;
	}

	toggle(TEST1_LED_PORT, TEST1_LED_PIN);
}


uint8_t USART__readData (uint8_t *data, uint8_t dataLength, uint8_t requester)
{
	uint8_t idxByte, dataSize;
	uint8_t retVal = E_NOT_OK;

	/* TODO: add defines */
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if ((requester & (USART0_data[0] & 0xF0)) == requester)
		{
			dataSize = USART0_data[0] & 0x0F;

			if (dataLength == dataSize)
			{
				for (idxByte = 0; idxByte < dataLength; idxByte++)
				{
					data[idxByte] = USART0_data[idxByte];
					USART0_data[idxByte] = 0;
				}

				retVal = E_OK;
			}
		}
	}
	return (retVal);
}


void USART__transmitData (uint8_t *data, uint8_t dataLength)
{
	uint8_t idxByte;

	Debug__setWhileState(WHILE_STATE_USART_BEFORE);
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
	Debug__setWhileState(WHILE_STATE_USART_AFTER);
}

#endif
