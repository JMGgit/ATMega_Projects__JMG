/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes.h"


static Mode_t currentMode;
static uint8_t modeOffTransition = FALSE;
static uint16_t timerModeChange;
uint8_t mode_EEPROM EEMEM;
RGB_Color_t Modes_currentColor = {255, 255, 255};


uint16_t timerModeChangeConf[MODE_NB] =
{
		0, 		/* MODE__STARTUP */
		0, 		/* MODE__OFF*/
		60000, 	/* MODE__BLENDING_SLOW_2_COLORS */
		30000, 	/* MODE__BLENDING_SWEEP_FAST */
		60000 	/* MODE__DOUBLE_COLOR*/
};

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
}


void Modes__Start (void)
{
	currentMode = eeprom_read_byte(&mode_EEPROM);

	if (currentMode == 0xFF)
	{
		currentMode = MODE__INIT;
	}
}


static void Modes__setNextMode (void)
{
	Modes__setMode(currentMode + 1);
}


uint8_t Modes__getMode (void)
{
	return currentMode;
}


static void Modes__updateColor (void)
{
	uint8_t USARTbuffer[USART_DATA_LENGTH_COLOR];

	if (E_OK == USART__readData(USARTbuffer, USART_DATA_LENGTH_COLOR, USART_REQESTER_COLOR))
	{
		Modes_currentColor.red = USARTbuffer[1];
		Modes_currentColor.green = USARTbuffer[2];
		Modes_currentColor.blue = USARTbuffer[3];
	}

	if (Buttons__isPressed(&buttonFunc1))
	{
		Modes_currentColor.red--;
	}

	if (Buttons__isPressed(&buttonFunc2))
	{
		Modes_currentColor.green--;
	}

	if (Buttons__isPressed(&buttonFunc3))
	{
		Modes_currentColor.blue--;
	}
}


static void Mode__eepromStorage (void)
{
	eeprom_update_byte(&mode_EEPROM, currentMode);
}


static void Modes__updateMatrix (void)
{
	Modes__updateColor();

	switch (currentMode)
	{
		case MODE__STARTUP:
		{
			Mode__Startup_x10();
			break;
		}

		case MODE__BLENDING_SLOW_2_COLORS:
		{
			ColorBlending__updateMatrix(BLENDING_MODE_SLOW_2_COLORS);
			break;
		}

		case MODE__BLENDING_SWEEP_FAST:
		{
			ColorBlending__updateMatrix(BLENDING_MODE_SWEEP_FAST);
			break;
		}

		case MODE__DOUBLE_COLOR:
		{
			LEDMatrix__clearMatrix();
			DoubleColor__UpdateMatrix_x10();
			break;
		}

		case MODE__OFF:
		{
			LEDMatrix__clearMatrix();

			if (!modeOffTransition)
			{
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
	Modes__Start();
#endif
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
		else
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

		if ((currentMode != MODE__OFF) && (modeOffTransition == FALSE))
		{
			if (Buttons__isPressedOnce(&buttonOff))
			{
				Modes__setMode(MODE__OFF);
				modeOffTransition = TRUE;
			}
		}
	}

	Modes__updateMatrix();
}