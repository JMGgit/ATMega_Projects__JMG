/*
 * Mode_Startup.c
 *
 *  Created on: 25.01.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Startup.h"
#include "Modes.h"


#define TEST_BRIGHTNESS_LEVEL	60

typedef enum
{
	TEST_INIT = 0,
	TEST_RED,
	TEST_GREEN,
	TEST_BLUE,
	TEST_WHITE,
	TEST_FINISHED
} testLedStates_N;

static uint8_t lineIt = 1;
static uint8_t colIt = 1;
static RGB_Color_t color = {TEST_BRIGHTNESS_LEVEL, 0, 0};
static testLedStates_N testLedState = TEST_INIT;


void Startup__x10 (void)
{
	switch (testLedState)
	{
		case TEST_INIT:
		{
			testLedState = TEST_RED;

			break;
		}

		case TEST_RED:
		{
			color =  LEDMatrix__getRGBColorFromComponents(TEST_BRIGHTNESS_LEVEL, 0, 0);
			LEDMatrix__setRGBColor(lineIt, colIt, color);

			if (colIt < LED_MATRIX_SIZE_COL)
			{
				colIt++;
			}
			else
			{
				if (lineIt < LED_MATRIX_SIZE_LIN)
				{
					lineIt++;
					colIt = 1;
				}
				else
				{
					testLedState = TEST_GREEN;
				}
			}

			break;
		}

		case TEST_GREEN:
		{
			color =  LEDMatrix__getRGBColorFromComponents(0, TEST_BRIGHTNESS_LEVEL, 0);
			LEDMatrix__setRGBColor(lineIt, colIt, color);

			if (colIt > 1)
			{
				colIt--;
			}
			else
			{
				if (lineIt > 1)
				{
					lineIt--;
					colIt = LED_MATRIX_SIZE_COL;
				}
				else
				{
					testLedState = TEST_BLUE;
				}
			}

			break;
		}

		case TEST_BLUE:
		{
			color =  LEDMatrix__getRGBColorFromComponents(0, 0, TEST_BRIGHTNESS_LEVEL);
			LEDMatrix__setRGBColor(lineIt, colIt, color);

			if (colIt < LED_MATRIX_SIZE_COL)
			{
				colIt++;
			}
			else
			{
				if (lineIt < LED_MATRIX_SIZE_LIN)
				{
					lineIt++;
					colIt = 1;
				}
				else
				{
					testLedState = TEST_WHITE;
				}
			}

			break;
		}

		case TEST_WHITE:
		{
			color =  LEDMatrix__getRGBColorFromComponents(TEST_BRIGHTNESS_LEVEL, TEST_BRIGHTNESS_LEVEL, TEST_BRIGHTNESS_LEVEL);
			LEDMatrix__setRGBColor(lineIt, colIt, color);

			if (colIt > 1)
			{
				colIt--;
			}
			else
			{
				colIt = LED_MATRIX_SIZE_COL;

				if (lineIt > 1)
				{
					lineIt--;
					colIt = LED_MATRIX_SIZE_COL;
				}
				else
				{
					testLedState = TEST_FINISHED;
				}
			}

			break;
		}

		default:
		{
			Modes__Start();
			break;
		}
	}
}
