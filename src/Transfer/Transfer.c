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
	uint8_t idx = 0;
	uint8_t mesIt, numberOfMes;
	uint16_t mesPoints, mesPointsIt;
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
			numberOfMes = DataLogger__getNumberOfMeasures();

			/* Byte 1 : number of measurements */
			usartWriteBuffer[idx++] = numberOfMes;

			for (mesIt = 1; mesIt <= numberOfMes; mesIt++)
			{
				/* number of measurements points */
				mesPoints = DataLogger__getNumberOfStoredValuesOfMeasure(mesIt);
				usartWriteBuffer[idx++] = (uint8_t)((mesPoints & 0xFF00) >> 8);
				usartWriteBuffer[idx++] = mesPoints & 0xFF;

				for (mesPointsIt = 0; mesPointsIt < mesPoints; mesPointsIt++)
				{
					DataLogger__getValueWithTime(mesIt, mesPointsIt, &year, &month, &date, &hour, &min, &sec, &data);
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
