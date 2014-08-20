/*
 * Mode_ColorBlending.c
 *
 *  Created on: 04.06.2013
 *      Author: Jean-Martin George
 */

#include "Mode_ColorBlending.h"
#include "Modes.h"

#define LEFT_RIGHT	0
#define RIGHT_LEFT	1


static RGB_Color_t currentColor = {0, 0, 0};
static RGB_Color_t startColor = {0, 0, 0};
static RGB_Color_t endColor = {0, 0, 0};
static uint16_t timeCounter = 0;
static uint8_t colorIt = 0;
static RGB_Color_t colorItTable[LED_MATRIX_SIZE_COL];
static uint8_t tableIt = 0;
static uint8_t direction = LEFT_RIGHT;
static uint8_t timerColorChange = 0;
static uint8_t colorStep = 0;
static uint16_t updateCounter = 0;
static uint16_t updateTimer = 0;
static uint8_t pause = FALSE;


RGB_Color_t ColorBlending__getCurrentColor (void)
{
	RGB_Color_t color;

	color.red 		= currentColor.red;
	color.green		= currentColor.green;
	color.blue 		= currentColor.blue;

	return color;
}


RGB_Color_t ColorBlending__getCurrentColorCol (uint8_t col)
{
	RGB_Color_t color;

	color.red 		= colorItTable[col].red;
	color.green		= colorItTable[col].green;
	color.blue 		= colorItTable[col].blue;

	return color;
}

void ColorBlending__updateMatrix (void)
{
	uint16_t linIt, colIt;
	uint8_t colorFactor = 255;

	if (Modes__getMode() == MODE__BLENDING_SWEEP)
	{
		timerColorChange = 1;
		colorStep = 10;
		updateTimer = 1000;
	}
	else if (Modes__getMode() == MODE__BLENDING_SWEEP_FAST)
	{
		timerColorChange = 1;
		colorStep = 10;
		updateTimer = 1;
	}
	else
	{
		timerColorChange = 10;
		colorStep = 1;
		updateTimer = 1;
	}

	if (pause == TRUE)
	{
		updateCounter++;

		if (updateCounter >= updateTimer)
		{
			pause = FALSE;
		}
	}
	else
	{
		timeCounter++;

		if (timeCounter >= timerColorChange)
		{
			timeCounter = 0;

			if	((currentColor.red == endColor.red) && (currentColor.green == endColor.green) && (currentColor.blue == endColor.blue))
			{
				if (colorIt < (NUMBER_OF_BLENDING_COLORS - 1))
				{
					colorIt++;
				}
				else
				{
					colorIt = 0;
				}
			}

			startColor.red		= 	colorFactor * pgm_read_byte(&RGB_BlendingColors[(colorIt * 3)]);
			startColor.green 	= 	colorFactor * pgm_read_byte(&RGB_BlendingColors[(colorIt * 3) + 1]);
			startColor.blue 	=	colorFactor * pgm_read_byte(&RGB_BlendingColors[(colorIt * 3)+ 2]);

			if (colorIt < (NUMBER_OF_BLENDING_COLORS - 1))
			{
				endColor.red 		= colorFactor * pgm_read_byte(&RGB_BlendingColors[((colorIt + 1) * 3)]);
				endColor.green 		= colorFactor * pgm_read_byte(&RGB_BlendingColors[((colorIt + 1) * 3) + 1]);
				endColor.blue 		= colorFactor * pgm_read_byte(&RGB_BlendingColors[((colorIt + 1) * 3) + 2]);
			}
			else
			{
				endColor.red 		= colorFactor * pgm_read_byte(&RGB_BlendingColors[0]);
				endColor.green 		= colorFactor * pgm_read_byte(&RGB_BlendingColors[1]);
				endColor.blue 		= colorFactor * pgm_read_byte(&RGB_BlendingColors[2]);
			}

			if (currentColor.red < endColor.red)
			{
				if ((currentColor.red + colorStep) < endColor.red)
				{
					currentColor.red = currentColor.red + colorStep;
				}
				else
				{
					currentColor.red = endColor.red;
				}
			}
			else
			{
				if (currentColor.red >  endColor.red)
				{
					if ((currentColor.red - colorStep) >  endColor.red)
					{
						currentColor.red = currentColor.red - colorStep;
					}
					else
					{
						currentColor.red = endColor.red;
					}
				}
			}

			if (currentColor.green < endColor.green)
			{
				if ((currentColor.green + colorStep) < endColor.green)
				{
					currentColor.green = currentColor.green + colorStep;
				}
				else
				{
					currentColor.green = endColor.green;
				}
			}
			else
			{
				if (currentColor.green >  endColor.green)
				{
					if ((currentColor.green - colorStep) >  endColor.green)
					{
						currentColor.green = currentColor.green - colorStep;
					}
					else
					{
						currentColor.green = endColor.green;
					}
				}
			}

			if (currentColor.blue < endColor.blue)
			{
				if ((currentColor.blue + colorStep) < endColor.blue)
				{
					currentColor.blue = currentColor.blue + colorStep;
				}
				else
				{
					currentColor.blue = endColor.blue;
				}
			}
			else
			{
				if (currentColor.blue >  endColor.blue)
				{
					if ((currentColor.blue - colorStep) >  endColor.blue)
					{
						currentColor.blue = currentColor.blue - colorStep;
					}
					else
					{
						currentColor.blue = endColor.blue;
					}
				}
			}

			if (direction == LEFT_RIGHT)
			{
				if (tableIt < (LED_MATRIX_SIZE_COL - 1))
				{
					tableIt++;
				}
				else
				{
					direction = RIGHT_LEFT;
					pause = TRUE;
					updateCounter = 0;
				}
			}
			else
			{
				if (tableIt > 0)
				{
					tableIt--;
				}
				else
				{
					direction = LEFT_RIGHT;
					pause = TRUE;
					updateCounter = 0;
				}
			}

			colorItTable[tableIt].red = currentColor.red;
			colorItTable[tableIt].green = currentColor.green;
			colorItTable[tableIt].blue = currentColor.blue;

		}

		for (linIt = 1; linIt <= LED_MATRIX_SIZE_LIN; linIt++)
		{
			for (colIt = 1; colIt <= LED_MATRIX_SIZE_COL; colIt++)
			{
				if ((Modes__getMode() == MODE__BLENDING_SWEEP) || (Modes__getMode() == MODE__BLENDING_SWEEP_FAST))
				{
					LEDMatrix__setRGBColor(linIt, colIt, colorItTable[colIt - 1]);
				}
				else if (Modes__getMode() == MODE__BLENDING)
				{
					LEDMatrix__setRGBColor(linIt, colIt, currentColor);
				}
				else
				{
					LEDMatrix__setRGBColor(linIt, colIt, currentColor);
				}
			}
		}
	}
}
