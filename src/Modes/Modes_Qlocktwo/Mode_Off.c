/*
 * Mode_Off.c
 *
 *  Created on: 07.04.2016
 *      Author: Jean-Martin George
 */


#include "Mode_Off.h"
#include "Modes.h"

extern void Qtwo__setNextLang (void);
extern void Modes__setStartup (uint8_t enabled);

uint8_t startupOn;

void Off__x10 (void)
{
#if (BUTTON_FUNC3_AVAILABLE == BUTTON_FUNC3_AVAILABLE_YES)
	static uint8_t langTimer = 255;
	static uint8_t startupTimer = 255;
#endif

#if (BUTTON_OFF_AVAILABLE == BUTTON_OFF_AVAILABLE_FUNC2)
	if (Buttons__isPressedOnce(&buttonFunc2))
	{
		Modes__Start();
	}
#endif

	if (Buttons__isPressedOnce(&buttonOff))
	{
		Modes__Start();
	}

#if (BUTTON_FUNC3_AVAILABLE == BUTTON_FUNC3_AVAILABLE_YES)
	if (Buttons__isPressed(&buttonFunc1))
	{
		if (startupTimer > 0)
		{
			startupTimer--;
		}
		else
		{
			if (startupOn)
			{
				startupOn = FALSE;
			}
			else
			{
				startupOn = TRUE;
			}

			startupTimer = 255;
			Modes__setStartup(startupOn);
			Modes__init();
		}
	}
	else
	{
		startupTimer = 255;
	}

	if (Buttons__isPressed(&buttonFunc2))
	{
		if (langTimer > 0)
		{
			langTimer--;
		}
		else
		{
			langTimer = 255;
			Qtwo__setNextLang();
			Modes__setMode(MODE__QLOCKTWO);
		}
	}
	else
	{
		langTimer = 255;
	}
#endif
}
