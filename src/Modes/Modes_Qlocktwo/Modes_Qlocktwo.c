/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes_Qlocktwo.h"


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
	Modes__setMode(MODE__QLOCKTWO, TRUE);
}


static void Modes__setNextMode (void)
{
	Modes__setMode(currentMode + 1, TRUE);
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

	if (eeprom_read_byte(&startupOn_EEPROM) == TRUE)
	{
		Modes__setMode(MODE__STARTUP, FALSE);
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
	if ((currentMode != MODE__OFF) && (currentMode != MODE__STARTUP))
	{
		if (Buttons__isPressedOnce(&buttonMode))
		{
			Modes__setNextMode();
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
