/*
 * Main.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Main.h"


int main (void)
{
	Drivers__init();
	Buttons__init();
	Clock__init();
#if (LCD_CONTROLLER != LCD_CONTROLLER_OFF)
	Lcd__init();
#endif
	Modes__init();

	setOutput(TEST_LED_DDR, TEST_LED_PIN);

	while (1)
	{
		if (uC__isTaskTrigger_x10())
		{
			Drivers__x10();
#if (LCD_CONTROLLER != LCD_CONTROLLER_OFF)
			Lcd__x10();
#endif
			Clock__x10();
			Buttons__x10();
			Modes__x10();
		}
	}

	return 0;
}
