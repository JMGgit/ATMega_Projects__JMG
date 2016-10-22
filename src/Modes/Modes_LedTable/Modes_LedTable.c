/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes_LedTable.h"


static Mode_t currentMode;
static uint16_t timerModeChange;
uint8_t mode_EEPROM EEMEM;
static uint8_t startupOn;
static uint8_t startupOn_EEPROM EEMEM;


uint16_t timerModeChangeConf[MODE_NB] =
{
		0xFFFF,	/* MODE__STARTUP = 0 */
		0xFFFF,	/* MODE__OFF */
		0xFFFF,	/* MODE__ALL_ON */
		0xFFFF,	/* MODE__BLENDING_SLOW */
		0xFFFF, /* MODE__BLENDING_SLOW_2_COLORS */
		0xFFFF,	/* MODE__BLENDING_FAST */
		0xFFFF,	/* MODE__BLENDING_FAST_2_COLORS */
		0xFFFF,	/* MODE__BLENDING_SWEEP */
		0xFFFF,	/* MODE__BLENDING_SWEEP_FAST */
		0xFFFF,	/* MODE__BLENDING_CLOCK */
		0xFFFF,	/* MODE__BLENDING_CLOCK_INVERTED */
		0xFFFF,	/* MODE__CLOCK */
		0xFFFF,	/* MODE__SNAKE */
};


static void Modes__transition (void)
{
	if (currentMode == MODE__SNAKE)
	{
		Snake__init();
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
		currentMode = MODE__INIT;
	}

	Modes__transition();
}


void Modes__Start (void)
{
	Modes__setMode(eeprom_read_byte(&mode_EEPROM));
}


static void Modes__setNextMode (void)
{
	Modes__setMode(currentMode + 1);
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

		case MODE__ALL_ON:
		{
			//AllOn__x10();
			Modes__setNextMode();
			break;
		}

		case MODE__BLENDING_SLOW:
		{
			ColorBlending__x10(BLENDING_MODE_SLOW);
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

		case MODE__BLENDING_SWEEP:
		{
			ColorBlending__x10(BLENDING_MODE_SWEEP);
			break;
		}

		case MODE__BLENDING_FAST:
		{
			ColorBlending__x10(BLENDING_MODE_FAST);
			break;
		}

		case MODE__BLENDING_SWEEP_FAST:
		{
			ColorBlending__x10(BLENDING_MODE_SWEEP_FAST);
			break;
		}

		case MODE__BLENDING_CLOCK:
		{
			ModeClock__x10(CLOCK_MODE_COLOR_BLENDING);
			break;
		}

		case MODE__BLENDING_CLOCK_INVERTED:
		{
			ModeClock__x10(CLOCK_MODE_COLOR_BLENDING_INVERTED);
			break;
		}

		case MODE__BLENDING_CLOCK_FAST:
		{
			ModeClock__x10(CLOCK_MODE_COLOR_BLENDING_FAST);
			break;
		}

		case MODE__BLENDING_CLOCK_INVERTED_FAST:
		{
			ModeClock__x10(CLOCK_MODE_COLOR_BLENDING_INVERTED_FAST);
			break;
		}

		case MODE__CLOCK:
		{
			//ModeClock__x10(CLOCK_MODE_ONE_COLOR);
			Modes__setNextMode();
			break;
		}

		case MODE__SNAKE:
		{
			Snake__x10(SNAKE_BRIGHTNESS_LEVEL);
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
		Modes__setMode(MODE__STARTUP);
		startupOn = TRUE;
	}
	else
	{
		Modes__Start();
		startupOn = FALSE;
		eeprom_update_byte(&startupOn_EEPROM, startupOn);
	}

	ModeClock__init();
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
			if (timerModeChange < timerModeChangeConf[currentMode])
			{
				timerModeChange++;
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
			Modes__setMode(MODE__OFF);
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
