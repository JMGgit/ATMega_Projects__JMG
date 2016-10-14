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
	Debug__setMainState(MAIN_STATE_INIT_BEGIN);
#if (DEBUG_MODE == DEBUG_MODE_ON)
	Debug__setMainState(MAIN_STATE_INIT_FAILUREMEMORY);
	FailureMemory__init();
#endif
	Debug__setMainState(MAIN_STATE_INIT_DRIVERS);
	Drivers__init();
	Debug__setMainState(MAIN_STATE_INIT_BUTTONS);
	Buttons__init();
#if (CLOCK_TYPE != CLOCK_TYPE_OFF)
	Debug__setMainState(MAIN_STATE_INIT_CLOCK);
	Clock__init();
#endif
#if (LCD_CONTROLLER != LCD_CONTROLLER_OFF)
	Debug__setMainState(MAIN_STATE_INIT_LCD);
	Lcd__init();
#endif
#ifdef LED_MATRIX_SIZE_LIN
#ifdef LED_MATRIX_SIZE_COL
	Debug__setMainState(MAIN_STATE_INIT_LEDMATRIX);
	LEDMatrix__init();
#endif
#endif
#if (PROJECT != PROJECT__IRMP)
	Debug__setMainState(MAIN_STATE_INIT_MODES);
	Modes__init();
#endif

	while (1)
	{
#if (PROJECT == PROJECT__LED_TABLE)
		if (ledTableUSARTmode)
		{
			Debug__setMainState(MAIN_STATE_X10_LEDLOOP);
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
				Debug__setMainState(MAIN_STATE_X10_BEGIN);
#if (DEBUG_MODE == DEBUG_MODE_ON)
				Debug__setMainState(MAIN_STATE_X10_DEBUG);
				Debug__captureData();
#endif
				Debug__setMainState(MAIN_STATE_X10_DRIVERS_UC_TASK);
				uC__resetTaskTrigger_x10();
				Debug__setMainState(MAIN_STATE_X10_DRIVERS_BEGIN);
				Drivers__begin_x10();
#if (LCD_CONTROLLER != LCD_CONTROLLER_OFF)
				Debug__setMainState(MAIN_STATE_X10_LCD);
				Lcd__x10();
#endif
#if (CLOCK_TYPE != CLOCK_TYPE_OFF)
				Debug__setMainState(MAIN_STATE_X10_CLOCK);
				Clock__x10();
#endif
				Debug__setMainState(MAIN_STATE_X10_BUTTONS);
				Buttons__x10();
#if (PROJECT != PROJECT__IRMP)
				Debug__setMainState(MAIN_STATE_X10_MODES);
				Modes__x10();
#endif
				Debug__setMainState(MAIN_STATE_X10_DRIVERS_END);
				Drivers__end_x10();
				Debug__setMainState(MAIN_STATE_X10_END);
			}
		}
	}

	return 0;
}
