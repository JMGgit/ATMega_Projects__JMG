/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes.h"


static Mode_t currentMode;
static uint8_t modeOffTransition = FALSE;
uint8_t mode_EEPROM EEMEM;
RGB_Color_t Modes_currentColor = {255, 255, 255};


static void Modes__transition (void)
{
#if (MODE_SNAKE == MODE_SNAKE_ON)
	Snake__init();
#endif
}


void Modes__setMode (Mode_t mode)
{
	if (mode < MODE_NB)
	{
		currentMode = mode;
	}
	else
	{
		currentMode = MODE__ALL_ON;
	}

	Modes__transition();
}


void Modes__Start (void)
{
	currentMode = eeprom_read_byte(&mode_EEPROM);

	if (currentMode == 0xFF)
	{
		currentMode = MODE__BLENDING_CLOCK;
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

		case MODE__ALL_ON:
		{
			//AllOn__updateMatrix();
			Modes__setNextMode();
			break;
		}

		case MODE__BLENDING:
		case MODE__BLENDING_SWEEP:
		case MODE__BLENDING_SWEEP_FAST:
		{
			ColorBlending__updateMatrix();
			break;
		}

		case MODE__BLENDING_CLOCK:
		{
			ColorBlending__updateMatrix();
			Clock__updateMatrix(CLOCK_MODE_COLOR_BLENDING);
			break;
		}

		case MODE__BLENDING_CLOCK_INVERTED:
		{
			ColorBlending__updateMatrix();
			Clock__updateMatrix(CLOCK_MODE_COLOR_BLENDING_INVERTED);
			break;
		}

		case MODE__CLOCK:
		{
			//Clock__updateMatrix(CLOCK_MODE_ONE_COLOR);
			Modes__setNextMode();
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
					Modes__setMode(MODE__ALL_ON);
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
	Modes__setMode(MODE__ALL_ON);
#endif
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
