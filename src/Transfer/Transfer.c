/*
 * Transfer.c
 *
 *  Created on: 10.06.2014
 *      Author: Jean-Martin George
 */


#include "Transfer.h"


#define TRANSFER_MODE__IDLE				1
#define TRANSFER_MODE__BUSY				2

static uint8_t trMode;
static uint8_t progress;


void Transfer__init(void)
{
	trMode = TRANSFER_MODE__IDLE;
}


uint8_t usartReadBuffer[USART_DATA_LENGTH_TRANSFER_READ];
uint8_t usartWriteBuffer[USART_DATA_LENGTH_TRANSFER_WRITE];

void Transfer__x10 (void)
{
	uint16_t idx = 0;
	uint8_t measIt, numberOfMeas;
	uint16_t measPoints, measPointsIt;
	uint8_t month, date, hour, min, sec;
	uint16_t year;
	uint16_t data;
	uint8_t negative, t_int, t_frac;

	if (trMode == TRANSFER_MODE__IDLE)
	{
		if (E_OK == USART__readDataBytes(&usartReadBuffer[0], USART_DATA_LENGTH_TRANSFER_READ, USART_REQESTER_TRANSFER))
		{
			toggle(TEST_LED_PORT, TEST_LED_PIN);
			trMode = TRANSFER_MODE__BUSY;
		}

		if (trMode == TRANSFER_MODE__BUSY)
		{
			numberOfMeas = DataLogger__getNumberOfMeasures();

			/* Byte 1 : number of measurements */
			usartWriteBuffer[idx++] = numberOfMeas;

			for (measIt = 1; measIt <= numberOfMeas; measIt++)
			{
				if (idx + 2 >= USART_DATA_LENGTH_TRANSFER_WRITE)
				{
					USART__transmitDataBytes(usartWriteBuffer, idx);
					idx = 0;
				}

				/* number of measurements points */
				measPoints = DataLogger__getNumberOfStoredValuesOfMeasure(measIt);
				usartWriteBuffer[idx++] = (uint8_t)((measPoints & 0xFF00) >> 8);
				usartWriteBuffer[idx++] = measPoints & 0xFF;

				for (measPointsIt = 0; measPointsIt < measPoints; measPointsIt++)
				{
					if (idx + 10 >= USART_DATA_LENGTH_TRANSFER_WRITE)
					{
						USART__transmitDataBytes(usartWriteBuffer, idx);
						idx = 0;
					}

					DataLogger__getValueWithTime(measIt, measPointsIt, &year, &month, &date, &hour, &min, &sec, &data);
					Temperature__getValuesFromRaw(data, &negative, &t_int, &t_frac);
					usartWriteBuffer[idx++] = (uint8_t)((year & 0xFF00) >> 8);
					usartWriteBuffer[idx++] = year & 0xFF;
					usartWriteBuffer[idx++] = month;
					usartWriteBuffer[idx++] = date;
					usartWriteBuffer[idx++] = hour;
					usartWriteBuffer[idx++] = min;
					usartWriteBuffer[idx++] = sec;
					usartWriteBuffer[idx++] = t_int;
					usartWriteBuffer[idx++] = t_frac;
				}
			}

			USART__transmitDataBytes(usartWriteBuffer, idx);
			trMode = TRANSFER_MODE__IDLE;
		}
	}
}


uint8_t Transfer__getProgress (void)
{
	return progress;
}
