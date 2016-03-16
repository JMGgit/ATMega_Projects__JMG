/*
 * uC.c
 *
 *  Created on: 28.03.2013
 *      Author: Jean-Martin George
 */


#include "uC.h"


static uint8_t update10ms;


void uC__init (void)
{
	/* control LED */
	setOutput(UC_LED_DDR, UC_LED_PIN);

#if (RUNTIME_TEST != RUNTIME_TEST_OFF)
	setOutput(RUNTIME_LED_PORT, RUNTIME_LED_PIN);
	setOutput(RUNTIME_OSC_PORT, RUNTIME_OSC_PIN);
#endif

	/* counter */
	TCCR1B = (1 << WGM12); 						/* CTC counter */
	TCCR1B |= ((1 << CS11) | (1 << CS10)); 		/* prescaler: 64 */
	OCR1A = (F_CPU / 100) / 64 - 1;				/* interrupt every 10ms */
	TIMSK1 |= (1 << OCIE1A);

	/* enable interrupts */
	sei();

#if (SPI_SPEED != SPI_DISABLED)
	SPI__masterInit();
#endif
	TWI__masterInit();
#if (USART_DATA_LENGTH_READ_MAX != 0)
	USART__init();
#endif
	ADC__init();
}


ISR(TIMER1_COMPA_vect)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		static uint8_t test_led = 0;

		update10ms = TRUE;

		test_led++;

		if (test_led == 100)
		{
			setHigh(UC_LED_PORT, UC_LED_PIN);
		}
		else if (test_led == 120)
		{
			setLow(UC_LED_PORT, UC_LED_PIN);
			test_led = 0;
		}
	}
}

uint8_t uC__isTaskTrigger_x10 (void)
{
	uint8_t update = FALSE;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if (update10ms == TRUE)
		{
			update = TRUE;
		}
	}

	return update;
}


void uC__resetTaskTrigger_x10 (void)
{
	update10ms = FALSE;
}
