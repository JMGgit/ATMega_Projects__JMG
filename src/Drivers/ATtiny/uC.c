/*
 * uC.c
 *
 *  Created on: 05.02.2016
 *      Author: Jean-Martin George
 */

#include "uC.h"


void uC__init (void)
{
#if 0 /* no cyclic task and no UC_LED */
	/* counter */
	TCCR1 = (1 << CS13) | (1 << CS11) | (1 << CS10); 	/* prescaler: 1024 */
	OCR1A = (F_CPU / 100) / 1024 - 1;	/* interrupt every 10ms (200 000 cycles) */
	TIMSK = (1 << OCIE1A);

	setOutput(UC_LED_DDR, UC_LED_PIN);
#endif

	/* enable interrupts */
	sei();
}

#if 0 /* no cylic task and no UC_LED */
ISR(TIMER1_COMPA_vect)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		static uint8_t test_led = 0;

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

		TCNT1 = 0; /* reset counter */
	}
}
#endif
