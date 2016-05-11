/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes_Qlocktwo.h"


static Mode_t currentMode;
static uint8_t modeOffTransition = FALSE;
static uint8_t startupOn;
static uint8_t startupOn_EEPROM EEMEM;

static void Modes__transition (void)
{
#if (MODE_SNAKE == MODE_SNAKE_ON)
	if (currentMode == MODE__SNAKE)
	{
		Snake__init();
	}
#endif

	if (currentMode == MODE__QLOCKTWO)
	{
		Qtwo__modeTransition();
	}
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
	Modes__setMode(MODE__QLOCKTWO);
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
			Snake__x10();
			break;
		}
#endif

		case MODE__OFF:
		{
			LEDMatrix__clearMatrix();

			if (!modeOffTransition)
			{
				Off__x10();
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
	LEDMatrix__clearMatrix();

	if (eeprom_read_byte(&startupOn_EEPROM) == TRUE)
	{
		Modes__setMode(MODE__STARTUP);
		startupOn = TRUE;
	}
	else
	{
		Modes__Start();
		startupOn = FALSE;
	}

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


void Modes__toggleStartupMode (void)
{
	if (startupOn)
	{
		startupOn = FALSE;
	}
	else
	{
		startupOn = TRUE;
	}

	eeprom_update_byte(&startupOn_EEPROM, startupOn);
}
