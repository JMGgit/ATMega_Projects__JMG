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


static void Debug__captureData (void)
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


void Debug__getModeString (char *buffer)
{
	uint8_t mode;
	uint8_t idxBuffer = 0;

	mode = Modes__getMode();

	if (mode >= 10)
	{
		itoa(mode, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(mode, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ' ';
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


void Debug__x10 (void)
{
#if (USART_DATA_LENGTH_READ_MAX != 0)
	static uint16_t timer = 100; /* send every 1 second */
	char buffer[255];
	uint8_t taskIncrement;
#endif

	Debug__captureData();

#if (USART_DATA_LENGTH_READ_MAX != 0)
	if (timer - uC__getTaskIncrement() >= 0)
	{
		timer = timer - uC__getTaskIncrement();
	}
	else
	{
		timer = 100;

		Clock__getCompleteDateWithYearString(&buffer[0]);
		strcpy(&buffer[8], " ");
		Clock__getTimeWithSecondsString(&buffer[9]);
		strcpy(&buffer[17], " ");
		strcpy(&buffer[18], "Mode: ");
		Debug__getModeString(&buffer[24]);
		strcpy(&buffer[26], "\r");
		USART__sendString(buffer);
	}
#endif
}

#endif /* (DEBUG_MODE == DEBUG_MODE_ON) */
