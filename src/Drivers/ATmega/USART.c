/*
 * USART.c
 *
 *  Created on: 27.03.2013
 *      Author: Jean-Martin George
 */


#include "USART.h"


#if (USART_DATA_LENGTH_READ_MAX != 0)

#define BAUD0 1250000UL
#define UBRR0_VAL ((F_CPU + BAUD0 * 4) / (BAUD0 * 8) - 1)
#define BAUD0_REAL (F_CPU / (8 * (UBRR0_VAL + 1)))
#define BAUD0_ERROR ((BAUD0_REAL * 1000) / BAUD0)

#if ((BAUD0_ERROR < 990) || (BAUD0_ERROR > 1010))
  #error too high USART0 error rate!
#endif

#define BAUD1 1250000UL
#define UBRR1_VAL ((F_CPU + BAUD1 * 4) / (BAUD1 * 8) - 1)
#define BAUD1_REAL (F_CPU / (8 * (UBRR1_VAL + 1)))
#define BAUD1_ERROR ((BAUD1_REAL * 1000) / BAUD1)

#if ((BAUD1_ERROR < 990) || (BAUD1_ERROR > 1010))
  #error too high USART1 error rate!
#endif

static volatile uint8_t USART0_data[USART_DATA_LENGTH_READ_MAX];
static volatile uint8_t USART0_idxData = 0;
static volatile uint8_t USART0_currentDataSize = 0;


void USART__init (void)
{
	/*** USART 0 ***/

	/* baud rate */
	UCSR0A = (1 << U2X0);
	UBRR0L = UBRR0_VAL & 0xFF;
	UBRR0H = (UBRR0_VAL >> 8) & 0xFF;

	/* enable receiver with interrupt and transmitter */
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);

	/* asynchronous transmission, no parity, 1 stop bit, 8 data bits */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	/*** USART 1 ***/

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega1284P__)
	/* baud rate */
	UCSR1A = (1 << U2X1);
	UBRR1L = UBRR1_VAL & 0xFF;
	UBRR1H = (UBRR1_VAL >> 8) & 0xFF;

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


static inline void USART__sendByte (uint8_t data)
{
	Debug__setWhileState(WHILE_STATE_USART1_BEFORE);
	while (!(UCSR0A & (1 << UDRE0))) {/* wait for empty transmit buffer */;}
	Debug__setWhileState(WHILE_STATE_USART1_AFTER);

	UDR0 = data;
}


void USART__sendData (uint8_t *data, uint8_t dataLength)
{
	uint8_t idxByte;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (idxByte = 0; idxByte < dataLength; idxByte++)
		{
			USART__sendByte(data[idxByte]);
		}
	}
}


void USART__sendString (char *string)
{
	Debug__setWhileState(WHILE_STATE_USART2_BEFORE);
	while (*string)
	{
		/* send as long as string terminator not present */
		USART__sendByte(*string);
		string++;
	};
	Debug__setWhileState(WHILE_STATE_USART2_AFTER);
}


#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega1284P__)

uint8_t USART1__readData (uint8_t *data, uint8_t dataLength, uint8_t requester)
{
	/* not implemented for now. For runtime reason interrupt is implemented directly in module LEDScreen */
	return (E_NOT_OK);
}


static inline void USART1__sendByte (uint8_t data)
{
	Debug__setWhileState(WHILE_STATE_USART1_BEFORE);
	while (!(UCSR1A & (1 << UDRE1))) {/* wait for empty transmit buffer */;}
	Debug__setWhileState(WHILE_STATE_USART1_AFTER);

	UDR1 = data;
}


void USART1__sendData (uint8_t *data, uint8_t dataLength)
{
	uint8_t idxByte;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (idxByte = 0; idxByte < dataLength; idxByte++)
		{
			USART1__sendByte(data[idxByte]);
		}
	}
}


void USART1__sendString (char *string)
{
	Debug__setWhileState(WHILE_STATE_USART2_BEFORE);
	while (*string)
	{
		/* send as long as string terminator not present */
		USART1__sendByte(*string);
		string++;
	};
	Debug__setWhileState(WHILE_STATE_USART2_AFTER);
}

#endif

#endif
