/*
 * uC.c
 *
 *  Created on: 28.03.2013
 *      Author: Jean-Martin George
 */


#include "uC.h"
#if (DEBUG_MODE == DEBUG_MODE_ON)
#include "FailureMemory.h"
#endif

static volatile uint8_t update10ms;

#if (RUNTIME_TEST != RUNTIME_TEST_OFF)
uint8_t runtimeCounter;
#endif


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

#if (SPI_SPEED != SPI_DISABLED)
	SPI__masterInit();
#endif
#if (TWI_MODE == TWI_MODE_MASTER)
	TWI__masterInit();
#endif
#if (USART_DATA_LENGTH_READ_MAX != 0)
	USART__init();
#endif
	ADC__init();

	uC__enableWatchdog();

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
			update = TRUE;
		}
	}

	return update;
}


void uC__resetTaskTrigger_x10 (void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		update10ms = FALSE;
		wdt_reset(); /* reset watchdog */
	}
}


void uC__begin_x10 (void)
{
#if (RUNTIME_TEST != RUNTIME_TEST_OFF)
	setHigh(RUNTIME_OSC_PORT, RUNTIME_OSC_PIN); /* oscilloscope */

	if (runtimeCounter < 100)
	{
		runtimeCounter++;
	}
	else
	{
		runtimeCounter = 0;
		toggle(RUNTIME_LED_PORT, RUNTIME_LED_PIN); /* LED */
	}
#endif
}


void uC__end_x10 (void)
{
#if (RUNTIME_TEST != RUNTIME_TEST_OFF)
	setLow(RUNTIME_OSC_PORT, RUNTIME_OSC_PIN); /* oscilloscope */
#endif
}


void uC__triggerSwReset (void)
{
	/* enable watchdog if not enabled before */

	Debug__setWhileState(WHILE_STATE_UC_BEFORE);
	while (1)
	{
		/* SW reset triggered by watchdog */
	}
	Debug__setWhileState(WHILE_STATE_UC_AFTER);
}


void uC__enableWatchdog (void)
{
	WDTCSR = (1 << WDCE) | (1 << WDIE) | (1 << WDE) | (1 << WDP1) | (1 << WDP0); /* 120ms with interrupt and with reset */
	/* interrupts have to be enabled! */
}


void uC__disableWatchdog (void)
{
	cli(); /* disable interrupts */
	wdt_reset();
	MCUSR &= ~(1 << WDRF);
	WDTCSR = (1 << WDCE) | (1<< WDE);
	WDTCSR = 0;
	sei(); /* enable interrupts */
}


ISR(WDT_vect)
{
#if (DEBUG_MODE == DEBUG_MODE_ON)
	FailureMemory__enterFault(FAILURE_ID__WATCHDOG);
	FailureMemory__disableStorage();
#endif
}
