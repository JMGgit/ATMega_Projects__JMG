/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes.h"


static Mode_t currentMode;
static uint8_t modeOffTransition = FALSE;


static void Modes__transition (void)
{
#if (MODE_SNAKE == MODE_SNAKE_ON)
	Snake__init();
#endif
	Qtwo__modeTransition();
}


void Modes__setMode (Mode_t mode)
{
	if (mode < MODE_NB)
	{
		currentMode = mode;
	}
	else
	{
		currentMode = MODE__QLOCKTWO;
	}

	Modes__transition();
}


void Modes__Start (void)
{
	currentMode = MODE__QLOCKTWO;
}


static void Modes__setNextMode (void)
{
	Modes__setMode(currentMode + 1);
}


uint8_t Modes__getMode (void)
{
	return currentMode;
}


static void Mode__eepromStorage (void)
{
	/* mode is not stored */
}

static void Modes__updateMatrix (void)
{
	switch (currentMode)
	{
		case MODE__STARTUP:
		{
			Mode__Startup_x10();
			break;
		}

		case MODE__QLOCKTWO:
		{
			LEDMatrix__clearMatrix();
			Qtwo__main_x10();
			break;
		}

		case MODE__SECONDS:
		{
			LEDMatrix__clearMatrix();
			Qtwo__seconds_x10();
			break;
		}

		case MODE__TIME_SETUP:
		{
			LEDMatrix__clearMatrix();
			Qtwo__timeSetup_x10();
			break;
		}

#if (MODE_SNAKE == MODE_SNAKE_ON)
		case MODE__SNAKE:
		{
			Snake__updateMatrix();
			break;
		}
#endif

		case MODE__OFF:
		{
			LEDMatrix__clearMatrix();

			if (!modeOffTransition)
			{
#if (BUTTON_OFF_AVAILABLE == BUTTON_OFF_AVAILABLE_FUNC2)
				if (Buttons__isPressedOnce(&buttonFunc2))
				{
					Modes__setMode(MODE__QLOCKTWO);
				}
#endif

				if (Buttons__isPressedOnce(&buttonOff))
				{
					Modes__Start();
				}
			}
			else
			{
				modeOffTransition = FALSE;
			}
			break;
		}

		default:
		{
			break;
		}
	}

	if ((currentMode != MODE__STARTUP) && (currentMode != MODE__OFF))
	{
		Mode__eepromStorage();
	}
}


void Modes__init (void)
{
#if (MODE_STARTUP == MODE_STARTUP_ON)
	Modes__setMode(MODE__STARTUP);
#else
	Modes__setMode(MODE__QLOCKTWO);
#endif
	Qtwo__init();
}


void Modes__x10 (void)
{
	if (Buttons__isPressedOnce(&buttonMode))
	{
#if (BUTTON_OFF_AVAILABLE != BUTTON_OFF_AVAILABLE_NO)
		if (currentMode != MODE__OFF)
		{
			Modes__setNextMode();
		}
#else
		Modes__setNextMode();
#endif
	}

	if ((currentMode != MODE__OFF) && (modeOffTransition == FALSE))
	{
		if (Buttons__isPressedOnce(&buttonOff))
		{
			Modes__setMode(MODE__OFF);
			modeOffTransition = TRUE;
		}

#if (BUTTON_OFF_AVAILABLE == BUTTON_OFF_AVAILABLE_FUNC2)
		if (Buttons__isPressedOnce(&buttonFunc2))
		{
			Modes__setMode(MODE__OFF);
			modeOffTransition = TRUE;
		}
#endif
	}

	Modes__updateMatrix();
}
