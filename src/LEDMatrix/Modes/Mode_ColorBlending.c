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
static uint8_t timerColorChange = 0;
static uint8_t colorStep = 0;


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


void ColorBlending__calcCurrentColor (uint8_t timerColorChange, uint8_t colorStep)
{
	uint8_t colorFactor = 255;

	timeCounter++;

	if (timeCounter >= timerColorChange)
	{
		timeCounter = 0;

		if ((currentColor.red == endColor.red) && (currentColor.green == endColor.green) && (currentColor.blue == endColor.blue))
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

		if (tableIt < (LED_MATRIX_SIZE_COL - 1))
		{
			tableIt++;
		}
		else
		{
			tableIt = 0;
		}

		colorItTable[tableIt].red = currentColor.red;
		colorItTable[tableIt].green = currentColor.green;
		colorItTable[tableIt].blue = currentColor.blue;
	}
}


void ColorBlending__x10 (uint8_t blendingMode)
{
	uint16_t linIt, colIt;
	RGB_Color_t *l_currentColor;
	RGB_Color_t l_currentColorB;

	switch (blendingMode)
	{
		case BLENDING_MODE_SLOW:
		case BLENDING_MODE_SLOW_2_COLORS:
		{
			timerColorChange = 20;
			colorStep = 2;
			break;
		}

		case BLENDING_MODE_FAST:
		case BLENDING_MODE_FAST_2_COLORS:
		{
			timerColorChange = 1;
			colorStep = 1;
			break;
		}

		case BLENDING_MODE_SWEEP:
		{
			timerColorChange = 50;
			colorStep = 10;
			break;
		}

		case BLENDING_MODE_SWEEP_FAST:
		{
			timerColorChange = 1;
			colorStep = 10;
			break;
		}

		default:
		{
			timerColorChange = 20;
			colorStep = 1;
			break;
		}
	}

	ColorBlending__calcCurrentColor(timerColorChange, colorStep);

	if ((blendingMode == BLENDING_MODE_SWEEP) || (blendingMode == BLENDING_MODE_SWEEP_FAST))
	{
		for (colIt = 1; colIt <= LED_MATRIX_SIZE_COL; colIt++)
		{
			l_currentColor = &colorItTable[colIt - 1];

			for (linIt = 1; linIt <= LED_MATRIX_SIZE_LIN; linIt++)
			{
				LEDMatrix__setRGBColor(linIt, colIt, *l_currentColor);
			}
		}
	}
	else /* without sweep */
	{
		l_currentColor = &currentColor;

		if ((blendingMode == BLENDING_MODE_SLOW_2_COLORS) || (blendingMode == BLENDING_MODE_FAST_2_COLORS))
		{
			for (linIt = 1; linIt <= LED_MATRIX_SIZE_LIN; linIt++)
			{
				if ((linIt % 2 ) == 0)
				{
					l_currentColorB = *l_currentColor;
				}
				else
				{
					/* shift colors */
					l_currentColorB = LEDMatrix__getRGBColorFromComponents((*l_currentColor).blue,
																(*l_currentColor).red,
																(*l_currentColor).green);

				}

				for (colIt = 1; colIt <= LED_MATRIX_SIZE_COL; colIt++)
				{
					LEDMatrix__setRGBColor(linIt, colIt, l_currentColorB);
				}
			}
		}
		else
		{
			LEDMatrix__setRGBColorForMatrix(*l_currentColor);
		}
	}
}
