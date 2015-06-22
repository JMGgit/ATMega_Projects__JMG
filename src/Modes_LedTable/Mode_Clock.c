/*
 * Mode_Clock.c
 *
 *  Created on: 07.06.2013
 *      Author: Jean-Martin George
 */

#include "Mode_Clock.h"
#include "Mode_ColorBlending.h"
#include "Buttons.h"


#define OFFSET_COL_NUM_1		4
#define OFFSET_COL_NUM_2		8
#define OFFSET_COL_NUM_3		14
#define OFFSET_COL_NUM_4		18
#define OFFSET_LIN				5

#define OFFSET_COL_NUM_1_BIG	2
#define OFFSET_COL_NUM_2_BIG	7
#define OFFSET_COL_NUM_3_BIG	14
#define OFFSET_COL_NUM_4_BIG	19
#define OFFSET_LIN_BIG			4

#define SIZE_LIN				5
#define SIZE_COL				3
#define SIZE_LIN_BIG			7
#define SIZE_COL_BIG			4

#define CLOCK_TYPE_NORMAL		0x00
#define CLOCK_TYPE_BIG			0x01

static uint8_t clockSize = CLOCK_TYPE_BIG;
static uint8_t lineOffset = 0;
static uint8_t colOffset = 0;

const uint8_t numTable[SIZE_LIN * SIZE_COL * 10] PROGMEM =
{
	1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,
	0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
	1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1,
	1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
	1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1,
	1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1,
	1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
	1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
	1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
	1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1
};


const uint8_t numTable_big[SIZE_LIN_BIG * SIZE_COL_BIG * 10] PROGMEM =
{
	1, 1, 1, 1,
	1, 0, 0, 1,
	1, 0, 0, 1,
	1, 0, 0, 1,
	1, 0, 0, 1,
	1, 0, 0, 1,
	1, 1, 1, 1,

	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,

	1, 1, 1, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
	1, 1, 1, 1,
	1, 0, 0, 0,
	1, 0, 0, 0,
	1, 1, 1, 1,

	1, 1, 1, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
	0, 1, 1, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
	1, 1, 1, 1,

	1, 0, 0, 1,
	1, 0, 0, 1,
	1, 0, 0, 1,
	1, 1, 1, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,

	1, 1, 1, 1,
	1, 0, 0, 0,
	1, 0, 0, 0,
	1, 1, 1, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
	1, 1, 1, 1,

	1, 1, 1, 1,
	1, 0, 0, 0,
	1, 0, 0, 0,
	1, 1, 1, 1,
	1, 0, 0, 1,
	1, 0, 0, 1,
	1, 1, 1, 1,

	1, 1, 1, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,

	1, 1, 1, 1,
	1, 0, 0, 1,
	1, 0, 0, 1,
	1, 1, 1, 1,
	1, 0, 0, 1,
	1, 0, 0, 1,
	1, 1, 1, 1,

	1, 1, 1, 1,
	1, 0, 0, 1,
	1, 0, 0, 1,
	1, 1, 1, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
	1, 1, 1, 1,
};


void Clock__updateMatrix (uint8_t clockMode)
{
	uint8_t hour = Clock__getHours();
	uint8_t min = Clock__getMinutes();
	uint8_t itLin, itCol, index;
	const uint8_t *num1Table, *num2Table, *num3Table, *num4Table;
	RGB_Color_t ClockColor;
	uint8_t numPosLin, numPosCol;

	/* time setting */

	if (Buttons__isPressed(&buttonFunc1))
	{
		if (Buttons__isPressedOnce(&buttonUp))
		{
			Clock__incHours();
		}

		if (Buttons__isPressedOnce(&buttonDown))
		{
			Clock__decHours();
		}
	}

	if (Buttons__isPressed(&buttonFunc2))
	{
		if (Buttons__isPressedOnce(&buttonUp))
		{
			Clock__incMinutes();
		}

		if (Buttons__isPressedOnce(&buttonDown))
		{
			Clock__decMinutes();
		}
	}

	/* numbers position */

	if (Buttons__isPressedOnce(&buttonUp))
	{
		if (lineOffset > 0)
		{
			lineOffset--;
		}
		else
		{
			lineOffset = LED_MATRIX_SIZE_LIN - 1;
		}
	}

	if (Buttons__isPressedOnce(&buttonDown))
	{
		if (lineOffset < (LED_MATRIX_SIZE_LIN - 1))
		{
			lineOffset++;
		}
		else
		{
			lineOffset = 0;
		}
	}

	if (Buttons__isPressedOnce(&buttonLeft))
	{
		if (colOffset > 0)
		{
			colOffset--;
		}
		else
		{
			colOffset = LED_MATRIX_SIZE_COL - 1;
		}
	}

	if (Buttons__isPressedOnce(&buttonRight))
	{
		if (colOffset < (LED_MATRIX_SIZE_COL - 1))
		{
			colOffset++;
		}
		else
		{
			colOffset = 0;
		}
	}

	if ((clockMode == CLOCK_MODE_ONE_COLOR) || (clockMode == CLOCK_MODE_COLOR_BLENDING))
	{
		LEDMatrix__clearMatrix();
	}

	if (clockMode == CLOCK_MODE_ONE_COLOR)
	{
		ClockColor = Modes_currentColor;
	}
	else if (clockMode == CLOCK_MODE_COLOR_BLENDING)
	{
		ClockColor = ColorBlending__getCurrentColor();
	}
	else /* CLOCK_MODE_COLOR_BLENDING_INVERTED */
	{
		ClockColor.red = 0;
		ClockColor.green = 0;
		ClockColor.blue = 0;
	}

	if (clockSize == CLOCK_TYPE_NORMAL)
	{
		num1Table = &numTable[(SIZE_LIN * SIZE_COL) * (hour / 10)];
		num2Table = &numTable[(SIZE_LIN * SIZE_COL) * (hour % 10)];
		num3Table = &numTable[(SIZE_LIN * SIZE_COL) * (min / 10)];
		num4Table = &numTable[(SIZE_LIN * SIZE_COL) * (min % 10)];

		for (itCol = 0; itCol < SIZE_COL; itCol++)
		{
			for (itLin = 0; itLin < SIZE_LIN; itLin++)
			{
				index = (itLin * SIZE_COL) + itCol;

				if (pgm_read_byte(&num1Table[index]) == 1)
				{
					LEDMatrix__setRGBColor(itLin + OFFSET_LIN, itCol + OFFSET_COL_NUM_1, ClockColor);
				}

				if (pgm_read_byte(&num2Table[index]) == 1)
				{
					LEDMatrix__setRGBColor(itLin + OFFSET_LIN, itCol + OFFSET_COL_NUM_2, ClockColor);
				}

				if (pgm_read_byte(&num3Table[index]) == 1)
				{
					LEDMatrix__setRGBColor(itLin + OFFSET_LIN, itCol + OFFSET_COL_NUM_3, ClockColor);
				}

				if (pgm_read_byte(&num4Table[index]) == 1)
				{
					LEDMatrix__setRGBColor(itLin + OFFSET_LIN, itCol + OFFSET_COL_NUM_4, ClockColor);
				}
			}
		}
	}
	else
	{
		num1Table = &numTable_big[(SIZE_LIN_BIG * SIZE_COL_BIG) * (hour / 10)];
		num2Table = &numTable_big[(SIZE_LIN_BIG * SIZE_COL_BIG) * (hour % 10)];
		num3Table = &numTable_big[(SIZE_LIN_BIG * SIZE_COL_BIG) * (min / 10)];
		num4Table = &numTable_big[(SIZE_LIN_BIG * SIZE_COL_BIG) * (min % 10)];

		for (itCol = 0; itCol < SIZE_COL_BIG; itCol++)
		{
			for (itLin = 0; itLin < SIZE_LIN_BIG; itLin++)
			{
				index = (itLin * SIZE_COL_BIG) + itCol;

				if (pgm_read_byte(&num1Table[index]) == 1)
				{
					numPosLin = itLin + lineOffset + OFFSET_LIN_BIG;
					numPosCol = itCol + colOffset + OFFSET_COL_NUM_1_BIG;

					if (numPosLin > LED_MATRIX_SIZE_LIN)
					{
						numPosLin = numPosLin - LED_MATRIX_SIZE_LIN;
					}

					if (numPosCol > LED_MATRIX_SIZE_COL)
					{
						numPosCol = numPosCol - LED_MATRIX_SIZE_COL;
					}

					LEDMatrix__setRGBColor(numPosLin, numPosCol, ClockColor);
				}

				if (pgm_read_byte(&num2Table[index]) == 1)
				{
					numPosLin = itLin + lineOffset + OFFSET_LIN_BIG;
					numPosCol = itCol + colOffset + OFFSET_COL_NUM_2_BIG;

					if (numPosLin > LED_MATRIX_SIZE_LIN)
					{
						numPosLin = numPosLin - LED_MATRIX_SIZE_LIN;
					}

					if (numPosCol > LED_MATRIX_SIZE_COL)
					{
						numPosCol = numPosCol - LED_MATRIX_SIZE_COL;
					}

					LEDMatrix__setRGBColor(numPosLin, numPosCol, ClockColor);
				}

				if (pgm_read_byte(&num3Table[index]) == 1)
				{
					numPosLin = itLin + lineOffset + OFFSET_LIN_BIG;
					numPosCol = itCol + colOffset + OFFSET_COL_NUM_3_BIG;

					if (numPosLin > LED_MATRIX_SIZE_LIN)
					{
						numPosLin = numPosLin - LED_MATRIX_SIZE_LIN;
					}

					if (numPosCol > LED_MATRIX_SIZE_COL)
					{
						numPosCol = numPosCol - LED_MATRIX_SIZE_COL;
					}

					LEDMatrix__setRGBColor(numPosLin, numPosCol, ClockColor);
				}

				if (pgm_read_byte(&num4Table[index]) == 1)
				{
					numPosLin = itLin + lineOffset + OFFSET_LIN_BIG;
					numPosCol = itCol + colOffset + OFFSET_COL_NUM_4_BIG;

					if (numPosLin > LED_MATRIX_SIZE_LIN)
					{
						numPosLin = numPosLin - LED_MATRIX_SIZE_LIN;
					}

					if (numPosCol > LED_MATRIX_SIZE_COL)
					{
						numPosCol = numPosCol - LED_MATRIX_SIZE_COL;
					}

					LEDMatrix__setRGBColor(numPosLin, numPosCol, ClockColor);
				}
			}
		}
	}

	numPosLin = 6 + lineOffset;
	numPosCol = 12 + colOffset;

	if (numPosLin > LED_MATRIX_SIZE_LIN)
	{
		numPosLin = numPosLin - LED_MATRIX_SIZE_LIN;
	}

	if (numPosCol > LED_MATRIX_SIZE_COL)
	{
		numPosCol = numPosCol - LED_MATRIX_SIZE_COL;
	}

	LEDMatrix__setRGBColor(numPosLin, numPosCol, ClockColor);

	numPosLin = 8 + lineOffset;
	numPosCol = 12 + colOffset;

	if (numPosLin > LED_MATRIX_SIZE_LIN)
	{
		numPosLin = numPosLin - LED_MATRIX_SIZE_LIN;
	}

	if (numPosCol > LED_MATRIX_SIZE_COL)
	{
		numPosCol = numPosCol - LED_MATRIX_SIZE_COL;
	}

	LEDMatrix__setRGBColor(numPosLin, numPosCol, ClockColor);
}
