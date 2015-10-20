/*
 * Mode_DoubleColor.c
 *
 *  Created on: 17.10.2015
 *      Author: JMG
 */


#include "Mode_DoubleColor.h"

#define OFF								0
#define OFF_TO_ON						1
#define ON_TO_OFF						2
#define ON								3

#define UPDATE_TIME				30
#define COLOR_STEP				5
#define SNAKE_SIZE				7
#define SNAKE_TRANSITION_TIME	20


static RGB_Color_t colorCalc, offOnColorCalc, onOffColorCalc, colorTransition;
static uint8_t timer;
static uint8_t timeTransition;
static uint8_t snakeAcol = 0;
static uint8_t snakeBcol = LED_MATRIX_SIZE_COL - 1;
static uint8_t matrix[LED_MATRIX_SIZE_LIN][LED_MATRIX_SIZE_COL];


static void DoubleColor__updateMatrix (void)
{
	uint8_t it, colIdx, onMode;

	if (snakeAcol < LED_MATRIX_SIZE_COL)
	{
		snakeAcol++;
	}
	else
	{
		snakeAcol = 1; /* not 0 to avoid double time for first LED */
	}

	if (snakeBcol > 0)
	{
		snakeBcol--;
	}
	else
	{
		snakeBcol = LED_MATRIX_SIZE_COL - 1; /* not LED_MATRIX_SIZE_COL - 1 to avoid double time for first LED */
	}

	colIdx = 0;

	for (it = 0; it < SNAKE_SIZE; it++)
	{
		if (it == (SNAKE_SIZE - 1))
		{
			onMode = OFF_TO_ON;
		}
		else
		{
			onMode = ON;
		}

		if ((snakeAcol + it) < LED_MATRIX_SIZE_COL)
		{
			matrix[0][snakeAcol + it] = onMode;
		}
		else
		{
			matrix[0][colIdx] = onMode;
			colIdx++;
		}

		if ((snakeBcol - it) >= 0)
		{
			matrix[1][snakeBcol - it] = onMode;
		}
		else
		{
			/* colIdx already incremented */
			matrix[1][LED_MATRIX_SIZE_COL - colIdx] = onMode;
		}
	}
}


static void DoubleColor__updateVisibility (uint8_t timer)
{
	uint8_t linIt, colIt;
	uint8_t allColorsReady = TRUE;
	uint8_t colorStep;

	colorStep = (MAX(colorTransition.red, MAX(colorTransition.green, colorTransition.blue)) / timer) | 1;

	static uint8_t colorTransTimer = 0;

	if (colorTransTimer > 0)
	{
		colorTransTimer--;
		allColorsReady = FALSE;
	}
	else
	{
		if (colorCalc.red != 0)
		{
			if ((offOnColorCalc.red + colorStep) < colorCalc.red)
			{
				offOnColorCalc.red = offOnColorCalc.red + colorStep;
				allColorsReady = FALSE;
			}
			else
			{
				offOnColorCalc.red = colorCalc.red;
			}
		}

		if (colorCalc.green != 0)
		{
			if ((offOnColorCalc.green + colorStep) < colorCalc.green)
			{
				offOnColorCalc.green = offOnColorCalc.green + colorStep;
				allColorsReady = FALSE;
			}
			else
			{
				offOnColorCalc.green = colorCalc.green;
			}
		}

		if (colorCalc.blue != 0)
		{
			if ((offOnColorCalc.blue + colorStep) < colorCalc.blue)
			{
				offOnColorCalc.blue = offOnColorCalc.blue + colorStep;
				allColorsReady = FALSE;
			}
			else
			{
				offOnColorCalc.blue = colorCalc.blue;
			}
		}

		if (onOffColorCalc.red - colorStep > 0)
		{
			onOffColorCalc.red = onOffColorCalc.red - colorStep;
			allColorsReady = FALSE;
		}
		else
		{
			onOffColorCalc.red = 0;
		}

		if (onOffColorCalc.green - colorStep > 0)
		{
			onOffColorCalc.green = onOffColorCalc.green - colorStep;
			allColorsReady = FALSE;
		}
		else
		{
			onOffColorCalc.green = 0;
		}

		if (onOffColorCalc.blue - colorStep > 0)
		{
			onOffColorCalc.blue = onOffColorCalc.blue - colorStep;
			allColorsReady = FALSE;
		}
		else
		{
			onOffColorCalc.blue = 0;
		}

		colorTransTimer = (timer / (MAX(colorTransition.red, MAX(colorTransition.green, colorTransition.blue))));
	}

	if (allColorsReady == TRUE)
	{
		timeTransition = FALSE;

		for (linIt = 0; linIt < LED_MATRIX_SIZE_LIN; linIt++)
		{
			for (colIt = 0; colIt < LED_MATRIX_SIZE_COL; colIt++)
			{
				if (matrix[linIt][colIt] == ON_TO_OFF)
				{
					matrix[linIt][colIt] = OFF;
				}
				else if (matrix[linIt][colIt] == OFF_TO_ON)
				{
					matrix[linIt][colIt] = ON;
				}
			}
		}
	}
}


static void DoubleColor__updateLeds (void)
{
	uint8_t linIt, colIt;
	RGB_Color_t tempColor, tempOnOffColor, tempOffOnColor;

	for (linIt = 0; linIt < LED_MATRIX_SIZE_LIN; linIt++)
	{
		if ((linIt % 2) == 0)
		{
			tempColor = colorCalc;
			tempOnOffColor = onOffColorCalc;
			tempOffOnColor = offOnColorCalc;
		}
		else
		{
			/* shift colorCalc */
			tempColor = getRGBColorFromComponents(colorCalc.green, colorCalc.blue, colorCalc. red);
			tempOnOffColor = getRGBColorFromComponents(onOffColorCalc.green, onOffColorCalc.blue, onOffColorCalc. red);
			tempOffOnColor = getRGBColorFromComponents(offOnColorCalc.green, offOnColorCalc.blue, offOnColorCalc. red);
		}

		for (colIt = 0; colIt < LED_MATRIX_SIZE_COL; colIt++)
		{
			if (matrix[linIt][colIt] == ON)
			{
				LEDMatrix__setRGBColor(linIt + 1, colIt + 1, tempColor);
			}
			else if (matrix[linIt][colIt] == ON_TO_OFF)
			{
				LEDMatrix__setRGBColor(linIt + 1, colIt + 1, tempOnOffColor);
			}
			else if (matrix[linIt][colIt] == OFF_TO_ON)
			{
				LEDMatrix__setRGBColor(linIt + 1, colIt + 1, tempOffOnColor);
			}
		}
	}
}


static void DoubleColor__startTransition (void)
{
	timeTransition = TRUE;
	colorTransition = colorCalc;
	offOnColorCalc.red = 0;
	offOnColorCalc.green = 0;
	offOnColorCalc.blue = 0;
	onOffColorCalc.red = colorCalc.red;
	onOffColorCalc.green = colorCalc.green;
	onOffColorCalc.blue = colorCalc.blue;
}


static void DoubleColor__getLastVisibility (void)
{
	uint8_t linIt, colIt;

	for (linIt = 0; linIt < LED_MATRIX_SIZE_LIN; linIt++)
	{
		for (colIt = 0; colIt < LED_MATRIX_SIZE_COL; colIt++)
		{
			if (matrix[linIt][colIt] == ON)
			{
				matrix[linIt][colIt] = ON_TO_OFF;
			}
		}
	}
}


void DoubleColor__UpdateMatrix_x10 (void)
{
	ColorBlending__calcCurrentColor(UPDATE_TIME, COLOR_STEP);

	if (!timeTransition)
	{
		if (timer == 0)
		{
			colorCalc = ColorBlending__getCurrentColor();
			DoubleColor__startTransition();
			DoubleColor__getLastVisibility();
			DoubleColor__updateMatrix();
			timer = UPDATE_TIME;
		}
		else
		{
			timer--;
		}
	}

	DoubleColor__updateVisibility(SNAKE_TRANSITION_TIME);
	DoubleColor__updateLeds();
}

