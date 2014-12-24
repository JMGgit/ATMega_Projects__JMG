/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes.h"


static uint8_t currentMode;
uint8_t mode_EEPROM EEMEM;
RGB_Color_t Modes_currentColor = {255, 255, 255};


static void Modes__transition (void)
{
	uint16_t linIt, colIt;

	Snake__init();

	/* refresh matrix */
	/* TODO: add mode transition */
	for (linIt = 1; linIt <= LED_MATRIX_SIZE_LIN; linIt++)
	{
		for (colIt = 1; colIt <= LED_MATRIX_SIZE_COL; colIt++)
		{
			LEDMatrix__setRGBColor(linIt, colIt, getRGBColorFromComponents(0, 0, 0));
		}
	}
}


void Modes__setMode (uint8_t mode)
{
	if (mode <= MODE_NB)
	{
		currentMode = mode;
	}
	else
	{
		currentMode = 1;
	}

	Modes__transition();
}


static void Modes__eepromInit (void)
{
	currentMode = eeprom_read_byte(&mode_EEPROM);

	if (currentMode == 0xFF)
	{
		currentMode = MODE__BLENDING_CLOCK_INVERTED;
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


void Modes__init (void)
{
	Modes__eepromInit();
}


void Modes__x10 (void)
{
	uint8_t USARTbuffer[USART_DATA_LENGTH_MODE];

	if ((Buttons__isPressedOnce(&buttonMode)) || (E_OK == USART__readData(USARTbuffer, USART_DATA_LENGTH_MODE, USART_REQESTER_MODE)))
	{
		Modes__setNextMode();
	}

	Modes__updateColor();

	switch (currentMode)
	{
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
			Modes__setNextMode();
			//Clock__updateMatrix(CLOCK_MODE_ONE_COLOR);
			break;
		}

		case MODE__SNAKE:
		{
			Snake__updateMatrix();
			break;
		}

		case MODE__ALL_ON:
		{
			Modes__setNextMode();
			//AllOn__updateMatrix();
			break;
		}

		default:
		{
			break;
		}
	}

	Mode__eepromStorage();
}
