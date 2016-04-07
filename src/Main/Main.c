/*
 * Main.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Main.h"

/* runtime test */
#if (RUNTIME_TEST != RUNTIME_TEST_OFF)
uint8_t runtimeCounter;
#endif

int main (void)
{
	Drivers__init();
	Buttons__init();
#if (CLOCK_TYPE != CLOCK_TYPE_OFF)
	Clock__init();
#endif
#if (LCD_CONTROLLER != LCD_CONTROLLER_OFF)
	Lcd__init();
#endif
#if (PROJECT != PROJECT__IRMP)
	Modes__init();
#endif

	while (1)
	{
		if (uC__isTaskTrigger_x10())
		{
#if (RUNTIME_TEST != RUNTIME_TEST_OFF)
			toggle(RUNTIME_OSC_PORT, RUNTIME_OSC_PIN); /* oscilloscope */
#endif
			Drivers__x10();
#if (LCD_CONTROLLER != LCD_CONTROLLER_OFF)
			Lcd__x10();
#endif
#if (CLOCK_TYPE != CLOCK_TYPE_OFF)
			Clock__x10();
#endif
			Buttons__x10();
#if (PROJECT != PROJECT__IRMP)
			Modes__x10();
#endif
			Drivers__end__x10();

			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				uC__resetTaskTrigger_x10();
			}

#if (RUNTIME_TEST != RUNTIME_TEST_OFF)
			if (runtimeCounter < 100)
			{
				runtimeCounter++;
			}
			else
			{
				runtimeCounter = 0;
				toggle(RUNTIME_LED_PORT, RUNTIME_LED_PIN); /* LED */
			}

			toggle(RUNTIME_OSC_PORT, RUNTIME_OSC_PIN); /* oscilloscope */
#endif
		}
	}

	return 0;
}
