/*
 * Main.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Main.h"

/* runtime test */
uint8_t runtimeCounter;


int main (void)
{
	Drivers__init();
	Buttons__init();
	Clock__init();
#if (LCD_CONTROLLER != LCD_CONTROLLER_OFF)
	Lcd__init();
#endif
	Modes__init();

	while (1)
	{
		if (uC__isTaskTrigger_x10())
		{
			/* runtime test */
			toggle(TEST3_LED_PORT, TEST3_LED_PIN); /* oscilloscope */
			Drivers__x10();
			/* runtime test */
			toggle(TEST3_LED_PORT, TEST3_LED_PIN); /* oscilloscope */
#if (LCD_CONTROLLER != LCD_CONTROLLER_OFF)
			Lcd__x10();
#endif
			Clock__x10();
			Buttons__x10();
			Modes__x10();

			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				uC__resetTaskTrigger_x10();
			}

			/* runtime test */
			toggle(TEST3_LED_PORT, TEST3_LED_PIN); /* oscilloscope */

			if (runtimeCounter < 100)
			{
				runtimeCounter++;
			}
			else
			{
				runtimeCounter = 0;
				toggle(TEST2_LED_PORT, TEST2_LED_PIN); /* LED */
			}
		}
	}

	return 0;
}
