/*
 * Mode_DoubleColor.c
 *
 *  Created on: 17.10.2015
 *      Author: JMG
 */


#include "Mode_DoubleColor.h"


static RGB_Color_t colorA, colorB;
static uint8_t timer = UPDATE_TIME;
static uint8_t snakeAcol = 1;
static uint8_t snakeBcol= LED_MATRIX_SIZE_COL;


void DoubleColor__UpdateMatrix_x10 (void)
{
	uint8_t it, colIdx;

	ColorBlending__calcCurrentColor(BLENDING_MODE_FAST);
	colorA = ColorBlending__getCurrentColor();
	colorB = getRGBColorFromComponents(colorA.blue, colorA.green, colorA.red); /* colorA shifted */

	if (timer == 0)
	{
		if (snakeAcol < LED_MATRIX_SIZE_COL)
		{
			snakeAcol++;
		}
		else
		{
			snakeAcol = 1;
		}

		if (snakeBcol > 1)
		{
			snakeBcol--;
		}
		else
		{
			snakeBcol = LED_MATRIX_SIZE_COL;
		}

		timer = UPDATE_TIME;
	}
	else
	{
		timer--;
	}

	LEDMatrix__clearMatrix();
	colIdx = 0;

	for (it = 0; it < SNAKE_SIZE; it++)
	{
		if ((snakeAcol + it) <= LED_MATRIX_SIZE_COL)
		{
			LEDMatrix__setRGBColor(1, snakeAcol + it, colorA);
		}
		else
		{
			colIdx++;
			LEDMatrix__setRGBColor(1, colIdx, colorA);
		}

		if ((snakeBcol - it) >= 1)
		{
			LEDMatrix__setRGBColor(2, snakeBcol - it, colorB);
		}
		else
		{
			/* colIdx already incremented */
			LEDMatrix__setRGBColor(2, LED_MATRIX_SIZE_COL - colIdx + 1, colorB);
		}
	}
}

