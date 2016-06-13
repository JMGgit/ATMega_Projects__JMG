/*
 * Main.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Main.h"


#if (PROJECT == PROJECT__LED_TABLE)
extern volatile uint8_t ledTableUSARTmode;
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
#ifdef LED_MATRIX_SIZE_LIN
#ifdef LED_MATRIX_SIZE_COL
	LEDMatrix__init();
#endif
#endif
#if (PROJECT != PROJECT__IRMP)
	Modes__init();
#endif

	while (1)
	{
#if (PROJECT == PROJECT__LED_TABLE)
		if (ledTableUSARTmode)
		{
			LEDScreen__loop();
		}
		else
#endif
		{
#if (PROJECT == PROJECT__LED_TABLE)
			LEDScreen__reset();
#endif

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
	}

	return 0;
}
