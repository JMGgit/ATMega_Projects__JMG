/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes_Qlocktwo.h"
#include "Mode_Qtwo.h"
#include "Mode_Startup.h"
#include "Mode_Snake.h"
#include "Mode_Off.h"
#include "Mode_FailureMemory.h"


static Mode_t currentMode;
static uint8_t startupOn;
static uint8_t startupOn_EEPROM EEMEM;


static void Modes__transition (void)
{
	if (currentMode == MODE__SNAKE)
	{
		Snake__init();
	}

	if (currentMode == MODE__QLOCKTWO)
	{
		Qtwo__modeTransition();
	}
}


void Modes__setMode (Mode_t mode, uint8_t transition)
{
	if (mode < MODE_NB)
	{
		currentMode = mode;
	}
	else
	{
		currentMode = MODE__QLOCKTWO;
	}

	if (transition)
	{
		Modes__transition();
	}
}

void Modes__Start (void)
{
#if (DEBUG_MODE == DEBUG_MODE_ON)
	if (FailureMemory__getFaultCounter() > 0)
	{
		Modes__setMode(MODE__FAILUREMEMORY, FALSE);
	}
	else
#endif
	{
		Modes__setMode(MODE__QLOCKTWO, TRUE);
	}
}


static void Modes__setNextMode (void)
{
	Modes__setMode(currentMode + 1, TRUE);
}


Mode_t Modes__getMode (void)
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
			Startup__x10();
			break;
		}

		case MODE__FAILUREMEMORY:
		{
			FailureMemory__x10();
			break;
		}

		case MODE__QLOCKTWO:
		{
			Qtwo__main_x10(QTWO_MODE_NORMAL);
			break;
		}

		case MODE__SECONDS:
		{
			Qtwo__main_x10(QTWO_MODE_SECONDS);
			break;
		}

		case MODE__TIME_SETUP:
		{
			Qtwo__main_x10(QTWO_MODE_SETUP);
			break;
		}

		case MODE__SNAKE:
		{
			Snake__x10(Qtwo__getCurrentBrightness());
			break;
		}


		case MODE__OFF:
		{
			Off__x10();
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
	Qtwo__init();

	if (eeprom_read_byte(&startupOn_EEPROM) == TRUE)
	{
		Modes__setMode(MODE__STARTUP, FALSE);
		startupOn = TRUE;
	}
	else
	{
		Modes__Start();
		startupOn = FALSE;
		eeprom_update_byte(&startupOn_EEPROM, startupOn);
	}
}


void Modes__x10 (void)
{
	if (		(currentMode != MODE__OFF)
			&& 	(currentMode != MODE__STARTUP)
	)
	{
		if (Buttons__isPressedOnce(&buttonMode))
		{
			if (currentMode != MODE__FAILUREMEMORY)
			{
				Modes__setNextMode();
			}
			else
			{
				Modes__setMode(MODE__QLOCKTWO, TRUE);
			}
		}

		if (Buttons__isPressedOnce(&buttonOff))
		{
			Modes__setMode(MODE__OFF, FALSE);
		}
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
