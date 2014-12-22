/*
 * Transfer.c
 *
 *  Created on: 10.06.2014
 *      Author: Jean-Martin George
 */


#include "Transfer.h"


#define TRANSFER_MODE__IDLE				1
#define TRANSFER_MODE__HEADER			2
#define TRANSFER_MODE__MEAS_LENGTH		3
#define TRANSFER_MODE__MEAS_VALUES		4

static uint8_t trMode;
static uint8_t progress;


void Transfer__init(void)
{
	trMode = TRANSFER_MODE__IDLE;
	progress = 0;
}


uint8_t usartReadBuffer[USART_DATA_LENGTH_TRANSFER_READ];
uint8_t usartWriteBuffer[USART_DATA_LENGTH_TRANSFER_WRITE];

void Transfer__x10 (void)
{
	uint16_t bufIdx = 0;
	static uint8_t measIt;
	uint8_t numberOfMeas;
	static uint16_t measPoints;
	static uint16_t measPointsIt;
	uint32_t measPointsCycle = 0;
	uint8_t month, date, hour, min, sec;
	uint16_t year;
	uint16_t data;
	uint8_t negative, t_int, t_frac;

	numberOfMeas = DataLogger__getNumberOfMeasures();


	switch (trMode)
	{
		case TRANSFER_MODE__IDLE:
		{
			progress = 0;

			if (E_OK == USART__readDataBytes(&usartReadBuffer[0], USART_DATA_LENGTH_TRANSFER_READ, USART_REQESTER_TRANSFER))
			{
				toggle(TEST_LED_PORT, TEST_LED_PIN);
				progress = 10;
				trMode = TRANSFER_MODE__HEADER;
			}
			break;
		}

		case TRANSFER_MODE__HEADER:
		{
			/* Byte 1 : number of measurements */
			usartWriteBuffer[bufIdx++] = numberOfMeas;
			USART__transmitDataBytes(usartWriteBuffer, bufIdx);
			bufIdx = 0;
			measIt = 1;
			progress = 20;
			trMode = TRANSFER_MODE__MEAS_LENGTH;
			break;
		}

		case TRANSFER_MODE__MEAS_LENGTH:
		{
			/* number of measurements points */
			measPoints = DataLogger__getNumberOfStoredValuesOfMeasure(measIt);
			usartWriteBuffer[bufIdx++] = (uint8_t)((measPoints & 0xFF00) >> 8);
			usartWriteBuffer[bufIdx++] = measPoints & 0xFF;
			USART__transmitDataBytes(usartWriteBuffer, bufIdx);
			bufIdx = 0;
			measPointsIt = 0;
			measPointsCycle = 0;
			progress = 30;
			trMode = TRANSFER_MODE__MEAS_VALUES;
			break;
		}

		case TRANSFER_MODE__MEAS_VALUES:
		{
			while ((measPointsIt < measPoints) && (measPointsCycle < 10))
			{
				DataLogger__getValueWithTime(measIt, measPointsIt, &year, &month, &date, &hour, &min, &sec, &data);
				Temperature__getValuesFromRaw(data, &negative, &t_int, &t_frac);
				usartWriteBuffer[bufIdx++] = (uint8_t)((year & 0xFF00) >> 8);
				usartWriteBuffer[bufIdx++] = year & 0xFF;
				usartWriteBuffer[bufIdx++] = month;
				usartWriteBuffer[bufIdx++] = date;
				usartWriteBuffer[bufIdx++] = hour;
				usartWriteBuffer[bufIdx++] = min;
				usartWriteBuffer[bufIdx++] = sec;
				usartWriteBuffer[bufIdx++] = t_int;
				usartWriteBuffer[bufIdx++] = t_frac;
				USART__transmitDataBytes(usartWriteBuffer, bufIdx);
				bufIdx = 0;
				measPointsIt++;
				measPointsCycle++;
			}

			if (measPointsIt >= measPoints)
			{
				measIt++;

				if (measIt > numberOfMeas)
				{
					trMode = TRANSFER_MODE__IDLE;
					progress = 100;
				}
				else
				{
					trMode = TRANSFER_MODE__MEAS_LENGTH;
				}
			}

			progress = 60;

			break;
		}

		default:
		{
			break;
		}
	}
}


uint8_t Transfer__getProgress (void)
{
	return progress;
}
