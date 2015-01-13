/*
 * IRMP.c
 *
 *  Created on: 23.12.2014
 *      Author: JMG
 */


#include <IRMP/IRMP_Appl.h>


#if (BUTTONS_IRMP == BUTTONS_IRMP_ON)

static void IRMP__testLed (uint8_t on)
{
	if (on)
	{
		setHigh(IRMP_LED_PORT, IRMP_LED_PIN);
	}
	else
	{
		setLow(IRMP_LED_PORT, IRMP_LED_PIN);
	}
}


void IRMP__init (void)
{
	/* lib */
	irmp_init();

	/* counter */
	TCCR0A = (1 << WGM01); 					/* CTC counter */
	TCCR0B |= (1 << CS01); 					/* prescaler: 8 */
	OCR0A = (F_CPU / F_INTERRUPTS) / 8 - 1;	/* interrupt every 15 000 cycles */
	TIMSK0 |= (1 << OCIE0A);

	/* callback to illuminate test LED */
	irmp_set_callback_ptr(&IRMP__testLed);
}


ISR(TIMER0_COMPA_vect)
{
	irmp_ISR();
}


uint8_t IRMP__readData (uint16_t address, uint8_t *data, uint8_t dataLength, uint8_t *repeat)
{
	uint8_t retVal = E_NOT_OK;
	IRMP_DATA IRMPData;

	if (dataLength > 2)
	{
		retVal = E_NOT_OK;
	}
	else
	{
		if (irmp_get_data(&IRMPData) == TRUE)
		{
			if (IRMPData.address == address)
			{
				if (dataLength == 1)
				{
					*data = (uint8_t)(IRMPData.command);
				}
				else
				{
					data[0] = (IRMPData.command) >> 8;
					data[1] = (IRMPData.command) & 0xFF;
				}

				if (IRMPData.flags & IRMP_FLAG_REPETITION)
				{
					*repeat = TRUE;
				}
			}

			retVal = E_OK;
		}
	}

	return retVal;
}


#endif
