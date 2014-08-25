/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes.h"


static Mode_t currentMode;

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

	Qtwo__modeTransition();
}


static void Modes__setNextMode (void)
{
	Modes__setMode(currentMode + 1);
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

		case MODE__OFF:
		{
			LEDMatrix__clearMatrix();

#if (OFF_BUTTON == OFF_BUTTON_FUNC2)
			if (Buttons__isPressedOnce(&buttonFunc2))
			{
				Modes__setMode(MODE__QLOCKTWO);
			}
#endif

			break;
		}

		default:
		{
			break;
		}
	}
}


void Modes__eepromStorage (void)
{
	Qtwo__eepromStorage();
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
		Modes__setNextMode();
	}

	Modes__updateMatrix();
}
