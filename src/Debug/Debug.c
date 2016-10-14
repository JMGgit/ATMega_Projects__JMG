/*
 * Debug.c
 *
 *  Created on: 09.10.2016
 *      Author: Jean-Martin George
 */

#include "Debug.h"


#if (DEBUG_MODE == DEBUG_MODE_ON)

#if (CLOCK_TYPE != CLOCK_TYPE_OFF)
static debugTime_t currentDebugTime;
#endif
uint8_t driversState;
uint8_t mainState;
uint8_t modeState;
uint8_t whileState;


void Debug__captureData (void)
{
#if (CLOCK_TYPE != CLOCK_TYPE_OFF)
	currentDebugTime.year = Clock__getYear();
	currentDebugTime.month = Clock__getMonth();
	currentDebugTime.date = Clock__getDate();
	currentDebugTime.hour = Clock__getHours();
	currentDebugTime.min = Clock__getMinutes();
	currentDebugTime.sec = Clock__getSeconds();
#endif
}


#if (CLOCK_TYPE != CLOCK_TYPE_OFF)
void Debug__getTime (debugTime_t *debugTime)
{
	memcpy(debugTime, &currentDebugTime, sizeof(debugTime_t));
}
#endif


#if (PROJECT != PROJECT__IRMP)
void Debug__getMode (Mode_t *mode)
{
	*mode = Modes__getMode();
}
#endif


void Debug__getMainState (uint8_t *state)
{
	*state = mainState;
}


void Debug__getDriversState (uint8_t *state)
{
	*state = driversState;
}


void Debug__getModeState (uint8_t *state)
{
	*state = modeState;
}


void Debug__getWhileState (uint8_t *state)
{
	*state = whileState;
}


#if (PROJECT == PROJECT__QLOCKTWO)

void Debug__getQtwoColor (RGB_Color_t *color)
{
	*color = Qtwo__getCurrentColor();
}


void Debug__getQtwoBrightness (uint8_t *brightness)
{
	*brightness = Qtwo__getCurrentBrightness();
}


#endif /* (PROJECT == PROJECT__QLOCKTWO) */

#endif /* (DEBUG_MODE == DEBUG_MODE_ON) */
