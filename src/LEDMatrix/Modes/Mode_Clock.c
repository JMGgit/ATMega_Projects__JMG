/*
 * Mode_Clock.c
 *
 *  Created on: 07.06.2013
 *      Author: Jean-Martin George
 */

#include "Mode_Clock.h"
#include "Mode_ColorBlending.h"
#include "Buttons.h"


#if (CLOCK_TYPE != CLOCK_TYPE_OFF)

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

#define CLOCK_SIZE_SMALL		0x00
#define CLOCK_SIZE_BIG			0x01

#define UPDATE_TIME				20
#define UPDATE_TIME_FAST		1
#define COLOR_STEP				2

static uint8_t clockSize, lineOffset, colOffset;

static uint8_t clockSize_EEPROM EEMEM = 0;
static uint8_t lineOffset_EEPROM EEMEM = 0;
static uint8_t colOffset_EEPROM EEMEM = 0;

static const uint8_t numTable[SIZE_LIN * SIZE_COL * 10] PROGMEM =
{
	1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, /* 0 */
	0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, /* 1 */
	1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, /* 2 */
	1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, /* 3 */
	1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, /* 4 */
	1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, /* 5 */
	1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, /* 6 */
	1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, /* 7 */
	1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, /* 8 */
	1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1  /* 9 */
};

static const uint8_t numTable_big[SIZE_LIN_BIG * SIZE_COL_BIG * 10] PROGMEM =
{
	1, 1, 1, 1,	1, 0, 0, 1,	1, 0, 0, 1,	1, 0, 0, 1,	1, 0, 0, 1,	1, 0, 0, 1,	1, 1, 1, 1, /* 0 */
	0, 0, 1, 0,	0, 0, 1, 0,	0, 0, 1, 0,	0, 0, 1, 0,	0, 0, 1, 0,	0, 0, 1, 0,	0, 0, 1, 0, /* 1 */
	1, 1, 1, 1,	0, 0, 0, 1,	0, 0, 0, 1,	1, 1, 1, 1,	1, 0, 0, 0,	1, 0, 0, 0,	1, 1, 1, 1, /* 2 */
	1, 1, 1, 1,	0, 0, 0, 1,	0, 0, 0, 1,	0, 1, 1, 1,	0, 0, 0, 1,	0, 0, 0, 1,	1, 1, 1, 1, /* 3 */
	1, 0, 0, 1,	1, 0, 0, 1,	1, 0, 0, 1,	1, 1, 1, 1,	0, 0, 0, 1,	0, 0, 0, 1,	0, 0, 0, 1, /* 4 */
	1, 1, 1, 1,	1, 0, 0, 0,	1, 0, 0, 0,	1, 1, 1, 1,	0, 0, 0, 1,	0, 0, 0, 1,	1, 1, 1, 1, /* 5 */
	1, 1, 1, 1,	1, 0, 0, 0,	1, 0, 0, 0,	1, 1, 1, 1,	1, 0, 0, 1,	1, 0, 0, 1,	1, 1, 1, 1, /* 6 */
	1, 1, 1, 1,	0, 0, 0, 1,	0, 0, 0, 1,	0, 0, 0, 1,	0, 0, 0, 1,	0, 0, 0, 1,	0, 0, 0, 1, /* 7 */
	1, 1, 1, 1,	1, 0, 0, 1,	1, 0, 0, 1,	1, 1, 1, 1,	1, 0, 0, 1,	1, 0, 0, 1,	1, 1, 1, 1, /* 8 */
	1, 1, 1, 1,	1, 0, 0, 1,	1, 0, 0, 1,	1, 1, 1, 1,	0, 0, 0, 1,	0, 0, 0, 1,	1, 1, 1, 1  /* 9 */
};


void ModeClock__init (void)
{
	if (eeprom_read_byte(&clockSize_EEPROM) == CLOCK_SIZE_BIG)
	{
		clockSize = CLOCK_SIZE_BIG;
	}
	else
	{
		clockSize = CLOCK_SIZE_SMALL;
		eeprom_update_byte(&clockSize_EEPROM, clockSize);
	}

	if (eeprom_read_byte(&lineOffset_EEPROM) <= LED_MATRIX_SIZE_LIN)
	{
		lineOffset = eeprom_read_byte(&lineOffset_EEPROM);
	}
	else
	{
		lineOffset = 0;
		eeprom_update_byte(&lineOffset_EEPROM, lineOffset);
	}

	if (eeprom_read_byte(&colOffset_EEPROM) <= LED_MATRIX_SIZE_COL)
	{
		colOffset = eeprom_read_byte(&colOffset_EEPROM);
	}
	else
	{
		colOffset = 0;
		eeprom_update_byte(&colOffset_EEPROM, colOffset);
	}
}


static void ModeClock__eepromStorage (void)
{
	eeprom_update_byte(&clockSize_EEPROM, clockSize);
	eeprom_update_byte(&lineOffset_EEPROM, lineOffset);
	eeprom_update_byte(&colOffset_EEPROM, colOffset);
}


void ModeClock__x10 (uint8_t clockMode)
{
	uint8_t hour = Clock__getHours();
	uint8_t min = Clock__getMinutes();
	uint8_t itLin, itCol, index, numPosLin, numPosCol;
	const uint8_t *num1Table, *num2Table, *num3Table, *num4Table;
	RGB_Color_t ClockColor;

	/* Buttons mapping:
	 * - Func1: size of digits
	 * - Func2: time setting - hours
	 * - Func3: time setting - minutes
	 * - Up/Down/Left/Right: position of digits */

	if (Buttons__isPressedOnce(&buttonFunc1))
	{
		if (clockSize == CLOCK_SIZE_SMALL)
		{
			clockSize = CLOCK_SIZE_BIG;
		}
		else
		{
			clockSize = CLOCK_SIZE_SMALL;
		}

		ModeClock__eepromStorage();
	}

#if ((PROJECT != PROJECT__LED_TABLE) || ((PROJECT != PROJECT__LED_TABLE) && (LEDTABLE_REVISION == LEDTABLE_REVISION_1)))
	if (Buttons__isPressedOnce(&buttonFunc2))
	{
		Clock__incHours();
	}

	if (Buttons__isPressedOnce(&buttonFunc3))
	{
		Clock__incMinutes();
	}
#endif

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

		ModeClock__eepromStorage();
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

		ModeClock__eepromStorage();
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

		ModeClock__eepromStorage();
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

		ModeClock__eepromStorage();
	}

	/* color calculation */

	if (clockMode == CLOCK_MODE_ONE_COLOR)
	{
		ClockColor = LEDMatrix__getColorFromInputs();
	}
	else if (clockMode == CLOCK_MODE_COLOR_BLENDING)
	{
		ColorBlending__calcCurrentColor(UPDATE_TIME, COLOR_STEP);
		ClockColor = ColorBlending__getCurrentColor();
	}
	else if (clockMode == CLOCK_MODE_COLOR_BLENDING_FAST)
	{
		ColorBlending__calcCurrentColor(UPDATE_TIME_FAST, COLOR_STEP);
		ClockColor = ColorBlending__getCurrentColor();
	}
	else if (clockMode == CLOCK_MODE_COLOR_BLENDING_INVERTED)
	{
		ColorBlending__calcCurrentColor(UPDATE_TIME, COLOR_STEP);
		ClockColor = LEDMatrix__getRGBColorFromComponents(0, 0, 0);
	}
	else if (clockMode == CLOCK_MODE_COLOR_BLENDING_INVERTED_FAST)
	{
		ColorBlending__calcCurrentColor(UPDATE_TIME_FAST, COLOR_STEP);
		ClockColor = LEDMatrix__getRGBColorFromComponents(0, 0, 0);
	}
	else
	{
		/* no other use cases */
	}

	/* no background color */
	if (		(clockMode == CLOCK_MODE_ONE_COLOR)
			|| (clockMode == CLOCK_MODE_COLOR_BLENDING)
			|| (clockMode == CLOCK_MODE_COLOR_BLENDING_FAST)
	)
	{
		LEDMatrix__clearMatrix();
	}
	else /* inverted mode */
	{
		LEDMatrix__setRGBColorForMatrix(ColorBlending__getCurrentColor());
	}

	/* digits */

	if (clockSize == CLOCK_SIZE_SMALL)
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
					numPosLin = itLin + lineOffset + OFFSET_LIN;
					numPosCol = itCol + colOffset + OFFSET_COL_NUM_1;

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
					numPosLin = itLin + lineOffset + OFFSET_LIN;
					numPosCol = itCol + colOffset + OFFSET_COL_NUM_2;

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
					numPosLin = itLin + lineOffset + OFFSET_LIN;
					numPosCol = itCol + colOffset + OFFSET_COL_NUM_3;

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
					numPosLin = itLin + lineOffset + OFFSET_LIN;
					numPosCol = itCol + colOffset + OFFSET_COL_NUM_4;

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
	else /* big size */
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

	/* hours/minutes separation ":" */

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

#endif
