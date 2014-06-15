/*
 * Transfer.c
 *
 *  Created on: 10.06.2014
 *      Author: Jean-Martin George
 */


#include "Transfer.h"


#define TRANSFER_MODE__IDLE				1
#define TRANSFER_MODE__BUSY				2

static uint8_t mode;
static uint8_t progress;


void Transfer__init(void)
{
	mode = TRANSFER_MODE__IDLE;
}


void Transfer__x10 (void)
{
	uint8_t usartBuffer[USART_DATA_LENGTH_TRANSFER];
	uint8_t idx = 0;

	if (mode == TRANSFER_MODE__IDLE)
	{
		if (E_OK == USART__readDataBytes(&usartBuffer[0], USART_DATA_LENGTH_TRANSFER, USART_REQESTER_TRANSFER))
		{
			/****** TEST ******/
			toggle(TEST_LED_PORT, TEST_LED_PIN);
			mode = TRANSFER_MODE__BUSY;
		}

		if (mode == TRANSFER_MODE__BUSY)
		{
			usartBuffer[idx++] = 0x01;
			usartBuffer[idx++] = DATA_LOGGER_MEASURES_NB;
			usartBuffer[idx++] = 0x02;
			usartBuffer[idx++] = (uint8_t)((MAX_MEASUREMENT_POINTS & 0xFF00) >> 8);
			usartBuffer[idx++] = MAX_MEASUREMENT_POINTS & 0xFF;
			usartBuffer[idx++] = 0x03;

			USART__transmitDataBytes(usartBuffer, idx);
			usartBuffer[idx++] = 0x04;

			mode = TRANSFER_MODE__IDLE;
		}
	}
}


uint8_t Transfer__getProgress (void)
{
	return progress;
}
