/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes_LedMirror.h"


static Mode_t currentMode;
static uint16_t timerModeChange;
uint8_t mode_EEPROM EEMEM;
static uint8_t startupOn;
static uint8_t startupOn_EEPROM EEMEM;


uint16_t timerModeChangeConf[MODE_NB] =
{
		0xFFFF,	/* MODE__STARTUP */
		0xFFFF,	/* MODE__OFF*/
		60000,	/* MODE__BLENDING_SLOW */
		60000, 	/* MODE__BLENDING_SLOW_2_COLORS */
		60000,	/* MODE__BLENDING_FAST */
		60000,	/* MODE__BLENDING_FAST_2_COLORS */
		60000, 	/* MODE__BLENDING_SWEEP_FAST */
		60000 	/* MODE__DOUBLE_COLOR*/
};


uint8_t Modes__getTaskIncrement (void)
{
	/* task update always every 10ms */
	return 1;
}


static void Modes__transition (void)
{
	/* nothing for now */
}


void Modes__setMode (Mode_t mode, uint8_t transition)
{
	if (mode < MODE_NB)
	{
		currentMode = mode;
	}
	else
	{
		currentMode = MODE__INIT;
	}

	if (transition)
	{
		Modes__transition();
	}
}


void Modes__Start (void)
{
	Modes__setMode(eeprom_read_byte(&mode_EEPROM), TRUE);
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
	eeprom_update_byte(&mode_EEPROM, currentMode);
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

		case MODE__BLENDING_SLOW:
		{
			ColorBlending__x10(BLENDING_MODE_SLOW);
			break;
		}

		case MODE__BLENDING_FAST:
		{
			ColorBlending__x10(BLENDING_MODE_FAST);
			break;
		}

		case MODE__BLENDING_SLOW_2_COLORS:
		{
			ColorBlending__x10(BLENDING_MODE_SLOW_2_COLORS);
			break;
		}

		case MODE__BLENDING_FAST_2_COLORS:
		{
			ColorBlending__x10(BLENDING_MODE_FAST_2_COLORS);
			break;
		}

		case MODE__BLENDING_SWEEP_FAST:
		{
			ColorBlending__x10(BLENDING_MODE_SWEEP_FAST);
			break;
		}

		case MODE__DOUBLE_COLOR:
		{
			//DoubleColor__x10();
			Modes__setNextMode();
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
		Modes__setMode(MODE__STARTUP, TRUE);
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
	if ((currentMode != MODE__OFF) && (currentMode != MODE__STARTUP))
	{
		if (Buttons__isPressedOnce(&buttonMode))
		{
			Modes__setNextMode();
			timerModeChange = 0;
		}
		else if (timerModeChangeConf[currentMode] != 0xFFFF)
		{
			if (timerModeChange  + uC__getTaskIncrement() <= timerModeChangeConf[currentMode])
			{
				timerModeChange = timerModeChange + uC__getTaskIncrement();
			}
			else
			{
				Modes__setNextMode();
				timerModeChange = 0;
			}
		}
		else
		{
			/* nothing to do */
		}

		if (Buttons__isPressedOnce(&buttonOff))
		{
			Modes__setMode(MODE__OFF, TRUE);
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
