/*
 * uC.c
 *
 *  Created on: 28.03.2013
 *      Author: Jean-Martin George
 */


#include "uC.h"


static volatile uint8_t update10ms;


void uC__init (void)
{
	/* control LED */
	setOutput(UC_LED_DDR, UC_LED_PIN);

	/* counter */
	TCCR1B = (1 << WGM12); 						/* CTC counter */
	TCCR1B |= ((1 << CS11) | (1 << CS10)); 		/* prescaler: 64 */
	OCR1A = 3124;								/* interrupt every 10ms (200 000 cycles) */
	TIMSK1 |= (1 << OCIE1A);

	/* SPI */
	SPI__masterInit();

	/* TWI (100 kHz) */
	TWI__masterInit();

	/* enable interrupts */
	sei();
}


ISR(TIMER1_COMPA_vect)
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


uint8_t uC__isTaskTrigger_x10 (void)
{
	uint8_t update = FALSE;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if (update10ms == TRUE)
		{
			update10ms = FALSE;
			update = TRUE;
		}
	}

	return update;
}
