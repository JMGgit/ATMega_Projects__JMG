/*
 * TWI.c
 *
 *  Created on: 27.03.2013
 *      Author: Jean-Martin George
 */


#include "TWI.h"


typedef enum
{
    TWI_ST_IDLE = 0,
    TWI_ST_TRANSMIT_START,
    TWI_ST_TRANSMIT_ADDRESS,
    TWI_ST_TRANSMIT_DATA,
    TWI_ST_TRANSMIT_STOP,
    TWI_ST_TRANSMIT_END,
    TWI_ST_RECEIVE_START,
    TWI_ST_RECEIVE_ADDRESS,
    TWI_ST_RECEIVE_DATA,
    TWI_ST_RECEIVE_STOP,
    TWI_ST_RECEIVE_END
} TWI_State_t;

static TWI_State_t TWI_State;


void TWI__masterInit (void)
{
	TWSR = 0; /* no prescaler */
	TWBR = 92; /* 100 kHz */
	/* 16 Mhz -> 72 */
	/* 20 Mhz -> 92 */
}


uint8_t TWI__transmitData (uint8_t *data, uint8_t dataLength, uint8_t slaveAddress)
{
	static uint8_t idxByte = 0;
	uint8_t retVal = E_NOT_OK;

	switch (TWI_State)
	{
	    case TWI_ST_IDLE:
	    {
	        TWI_State = TWI_ST_TRANSMIT_START;
	        retVal = E_PENDING;
	    }

	    case TWI_ST_TRANSMIT_START:
	    {
	        /* send start condition */
	        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	        TWI_State = TWI_ST_TRANSMIT_ADDRESS;
	        retVal = E_PENDING;
	        break;
	    }

	    case TWI_ST_TRANSMIT_ADDRESS:
	    {
	        /* assure last transmission is finished */
	        if (!(TWCR & (1 << TWINT)))
	        {
	            /* send slave address and write bit */
	            TWDR = (((slaveAddress) << 1) & 0xFE) | TWI_WRITE_BIT;
	            TWCR = (1 << TWINT) | (1 << TWEN);

	            TWI_State = TWI_ST_TRANSMIT_DATA;
	        }

	        retVal = E_PENDING;
	        break;
	    }

	    case TWI_ST_TRANSMIT_DATA:
	    {
	        /* assure last transmission is finished */
            if (!(TWCR & (1 << TWINT)))
            {
                /* send data (only one byte) */
                TWDR = data[idxByte];
                TWCR = (1 << TWINT) | (1 << TWEN);

                idxByte++;

                if (idxByte >= dataLength)
                {
                    TWI_State = TWI_ST_TRANSMIT_STOP;
                }
            }

            retVal = E_PENDING;
            break;
	    }

	    case TWI_ST_TRANSMIT_STOP:
	    {
            /* assure last transmission is finished */
            if (!(TWCR & (1 << TWINT)))
            {
                /* send STOP condition*/
                TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

                TWI_State = TWI_ST_TRANSMIT_END;
            }

            retVal = E_PENDING;
            break;
	    }

        case TWI_ST_TRANSMIT_END:
        {
            /* assure last transmission is finished */
            if (TWCR & (1 << TWSTO))
            {
                TWI_State = TWI_ST_IDLE;
            }

            retVal = E_OK;
            break;
        }

        default:
        {

        }
	}

	return retVal;
}


uint8_t TWI__readData (uint8_t *data, uint8_t dataLength, uint8_t slaveAddress)
{
	static uint8_t idxByte = 0;
	uint8_t retVal = E_NOT_OK;

    switch (TWI_State)
    {
        case TWI_ST_IDLE:
        {
            TWI_State = TWI_ST_RECEIVE_START;
            retVal = E_PENDING;
        }

        case TWI_ST_RECEIVE_START:
        {
            /* send start condition */
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

            TWI_State = TWI_ST_RECEIVE_ADDRESS;
            retVal = E_PENDING;
            break;
        }

        case TWI_ST_RECEIVE_ADDRESS:
        {
            /* assure last transmission is finished */
            if (!(TWCR & (1 << TWINT)))
            {
                /* send slave address and write bit */
                TWDR = (((slaveAddress) << 1) & 0xFE) | TWI_READ_BIT;
                TWCR = (1 << TWINT) | (1 << TWEN);

                TWI_State = TWI_ST_RECEIVE_DATA;
            }

            retVal = E_PENDING;
            break;
        }

        case TWI_ST_RECEIVE_DATA:
        {
            /* assure last transmission is finished */
            if (!(TWCR & (1 << TWINT)))
            {
                if (idxByte > 0)
                {
                    data[idxByte - 1] = TWDR;
                }

                if (idxByte < dataLength)
                {
                    /* request data (only one byte) */
                    if (idxByte < (dataLength - 1))
                    {
                        /* request more data -> ACK */
                        TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
                    }
                    else
                    {
                        /* last byte -> no ACK */
                        TWCR = (1 << TWINT) | (1 << TWEN);
                    }
                }
                else
                {
                    TWI_State = TWI_ST_RECEIVE_STOP;
                }

                idxByte++;
            }

            retVal = E_PENDING;
            break;
        }

        case TWI_ST_RECEIVE_STOP:
        {
            /* assure last transmission is finished */
            if (!(TWCR & (1 << TWINT)))
            {
                /* send STOP condition*/
                TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

                TWI_State = TWI_ST_RECEIVE_END;
            }

            retVal = E_PENDING;
            break;
        }

        case TWI_ST_RECEIVE_END:
        {
            /* assure last transmission is finished */
            if (TWCR & (1 << TWSTO))
            {
                TWI_State = TWI_ST_IDLE;
            }

            retVal = E_OK;
            break;
        }

        default:
        {

        }
    }

    return retVal;
}
