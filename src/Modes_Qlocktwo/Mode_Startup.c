/*
 * Mode_Startup.c
 *
 *  Created on: 25.01.2014
 *      Author: Jean-Martin George
 */


#include "Mode_Startup.h"
#include "Modes.h"

#define TEST_BRIGHTNESS	20

static uint8_t lineIt = 0;
static uint8_t colIt = 0;
static RGB_Color_t color = {TEST_BRIGHTNESS, 0, 0};
static testLedStates_N testLedState;


void Mode__Startup_x10 (void)
{
	switch (testLedState)
	{
		case TEST_RED:
		{
			color =  getRGBColorFromComponents(TEST_BRIGHTNESS, 0, 0);

			if (lineIt <= LED_MATRIX_SIZE_LIN)
			{
				if (colIt < LED_MATRIX_SIZE_COL)
				{
					colIt++;
				}
				else
				{
					colIt = 0;
					lineIt++;
				}
			}
			else
			{
				testLedState++;
			}

			break;
		}

		case TEST_GREEN:
		{
			color =  getRGBColorFromComponents(0, TEST_BRIGHTNESS, 0);

			if (lineIt > 0)
			{
				if (colIt > 0)
				{
					colIt--;
				}
				else
				{
					colIt = LED_MATRIX_SIZE_COL;
					lineIt--;
				}
			}
			else
			{
				testLedState++;
			}

			break;
		}

		case TEST_BLUE:
		{
			color =  getRGBColorFromComponents(0, 0, TEST_BRIGHTNESS);

			if (lineIt <= LED_MATRIX_SIZE_LIN)
			{
				if (colIt < LED_MATRIX_SIZE_COL)
				{
					colIt++;
				}
				else
				{
					colIt = 0;
					lineIt++;
				}
			}
			else
			{
				testLedState++;
			}

			break;
		}

		case TEST_WHITE:
		{
			color =  getRGBColorFromComponents(TEST_BRIGHTNESS, TEST_BRIGHTNESS, TEST_BRIGHTNESS);

			if (lineIt > 0)
			{
				if (colIt > 0)
				{
					colIt--;
				}
				else
				{
					colIt = LED_MATRIX_SIZE_COL;
					lineIt--;
				}
			}
			else
			{
				testLedState++;
			}
			break;
		}

		default:
		{
			Modes__setMode(MODE__QLOCKTWO);
		}
	}

	LEDMatrix__setRGBColor(lineIt, colIt, color);
}
