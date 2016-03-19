/*
 * TWI_Slave.c
 *
 *  Created on: 16.03.2013
 *      Author: Jean-Martin George
 */


#include "TWI_Slave.h"

#if (TWI_MODE == TWI_MODE_SLAVE)


#define TWI_STATUS_START_ACK		0xA8
#define TWI_STATUS_DATA_ACK			0xB8
#define TWI_STATUS_DATA_NO_ACK		0xC0


static volatile uint8_t twiDataIdx;
static uint8_t twiDataTxLength;
static uint8_t twiDataTxBuffer[TWI_DATA_LENGTH_MAX];
static uint8_t *twiDataTxPtr;
static volatile uint8_t twiBusyFlag;


void TWI__slaveInit (uint8_t *txData, uint8_t txDataLength)
{
	TWAR = (TWI_SLAVE_ADDRESS << 1);
	TWCR = (1 << TWEA)| (1 << TWEN) | (1 << TWIE);
	TWCR |= (1 << TWINT);
	twiDataTxPtr = txData;
	twiDataTxLength = txDataLength;
	twiBusyFlag = FALSE;

}


ISR(TWI_vect)
{
	switch (TWSR)
	{
		case TWI_STATUS_START_ACK:
		{
			twiBusyFlag = TRUE;
			/* slave transmitter mode enabled */
			twiDataIdx = 0;
			TWDR = twiDataTxBuffer[twiDataIdx];
			twiDataIdx++;
			break;
		}

		case TWI_STATUS_DATA_ACK:
		{
			if (twiDataIdx < twiDataTxLength)
			{
				TWDR = twiDataTxBuffer[twiDataIdx];
				twiDataIdx++;
			}

			break;
		}

		case TWI_STATUS_DATA_NO_ACK:
		{
			/* last byte has been transferred */
			twiBusyFlag = FALSE;
			break;
		}

		default:
		{
			/* do nothing */
			break;
		}
	}

	/* reset interrupt flag */
	TWCR |= (1 << TWINT);
}


void TWI__slaveUpdateTxData (void)
{
	while (twiBusyFlag == 0)
	{
		/* wait till current transaction is finished */
	}

	memcpy(twiDataTxBuffer, twiDataTxPtr, twiDataTxLength);
}


#endif
