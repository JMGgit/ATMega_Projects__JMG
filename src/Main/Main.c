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
			uC__resetTaskTrigger_x10();
			Drivers__begin_x10();
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
			Drivers__end_x10();
		}
	}

	return 0;
}
