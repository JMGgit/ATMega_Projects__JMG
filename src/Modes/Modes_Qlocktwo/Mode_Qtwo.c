/*
 * Mode_Qlocktwo.c
 *
 *  Created on: 19.09.2013
 *      Author: Jean-Martin George
 */

#include "Mode_Qtwo.h"
#include "Modes.h"


#define OFF								0
#define OFF_TO_ON						1
#define ON_TO_OFF						2
#define ON								3

#define QTWO_LINE_NB					10
#define QTWO_COL_NB						11

#define QTWO_BRIGHTNESS_NB				4
#define QTWO_COLOR_NB					7

#define QTWO_LDR_HYST					30

#define QTWO_TRANSITION_TIMER			50
#define QTWO_TRANSITION_TIMER_SEC		17
#define QTWO_TRANSITION_TIMER_SETUP		0

#define QTWO_SETUP_TIMER				50

#define DIGIT_SIZE_LIN					7
#define DIGIT_SIZE_COL					5

#define QTWO_COLOR_CHANGE_HOURS			2

#define QTWO_BRIGHTNESS_LOW				1
#define QTWO_BRIGHTNESS_HIGH			2

#define TIMER_COLOR_BUTTON				120

#define QTWO_LANG_DE_SUED				0
#define QTWO_LANG_DE					1
#define QTWO_LANG_FR					2
#define QTWO_LANG_EN					3
#define QTWO_LANG_NB					4

static const uint16_t ldrLevels[QTWO_BRIGHTNESS_NB] = {0, 150, 300, 600};

typedef enum
{
	STATE_1 = 0,
	STATE_1_TO_2,
	STATE_2,
	STATE_2_TO_3,
	STATE_2_TO_1,
	STATE_3,
	STATE_3_TO_2,
	STATE_3_TO_4,
	STATE_4,
	STATE_4_TO_3
} StateLDR_N;

StateLDR_N stateLDR = STATE_1;

static const uint8_t colors[3 * QTWO_COLOR_NB] =
{
		1, 0, 0,
		1, 1, 0,
		0, 1, 0,
		0, 1, 1,
		1, 0, 1,
		0, 0, 1,
		1, 1, 1
};

static const uint8_t brightnessLevels[QTWO_COLOR_NB][QTWO_BRIGHTNESS_NB] =
{
		{QTWO_BRIGHTNESS_LEVEL_1, QTWO_BRIGHTNESS_LEVEL_2, QTWO_BRIGHTNESS_LEVEL_3, QTWO_BRIGHTNESS_LEVEL_4},
		{QTWO_BRIGHTNESS_LEVEL_1, QTWO_BRIGHTNESS_LEVEL_2, QTWO_BRIGHTNESS_LEVEL_3, QTWO_BRIGHTNESS_LEVEL_4},
		{QTWO_BRIGHTNESS_LEVEL_1, QTWO_BRIGHTNESS_LEVEL_2, QTWO_BRIGHTNESS_LEVEL_3, QTWO_BRIGHTNESS_LEVEL_4},
		{QTWO_BRIGHTNESS_LEVEL_1, QTWO_BRIGHTNESS_LEVEL_2, QTWO_BRIGHTNESS_LEVEL_3, QTWO_BRIGHTNESS_LEVEL_4},
		{QTWO_BRIGHTNESS_LEVEL_1, QTWO_BRIGHTNESS_LEVEL_2, QTWO_BRIGHTNESS_LEVEL_3, QTWO_BRIGHTNESS_LEVEL_4},
		{QTWO_BRIGHTNESS_LEVEL_1, QTWO_BRIGHTNESS_LEVEL_2, QTWO_BRIGHTNESS_LEVEL_3, QTWO_BRIGHTNESS_LEVEL_4},
		/* dark blue */
		{(uint8_t)(2.50 * QTWO_BRIGHTNESS_LEVEL_1), (uint8_t)(1.50 * QTWO_BRIGHTNESS_LEVEL_2),
		 (uint8_t)(1.25 * QTWO_BRIGHTNESS_LEVEL_3), (uint8_t)(1.25 * QTWO_BRIGHTNESS_LEVEL_4)}
};

static const uint8_t brightnessLevels_high[QTWO_COLOR_NB][QTWO_BRIGHTNESS_NB] =
{
		{QTWO_BRIGHTNESS_LEVEL_BRIGHT_1, QTWO_BRIGHTNESS_LEVEL_BRIGHT_2, QTWO_BRIGHTNESS_LEVEL_BRIGHT_3, QTWO_BRIGHTNESS_LEVEL_BRIGHT_4},
		{QTWO_BRIGHTNESS_LEVEL_BRIGHT_1, QTWO_BRIGHTNESS_LEVEL_BRIGHT_2, QTWO_BRIGHTNESS_LEVEL_BRIGHT_3, QTWO_BRIGHTNESS_LEVEL_BRIGHT_4},
		{QTWO_BRIGHTNESS_LEVEL_BRIGHT_1, QTWO_BRIGHTNESS_LEVEL_BRIGHT_2, QTWO_BRIGHTNESS_LEVEL_BRIGHT_3, QTWO_BRIGHTNESS_LEVEL_BRIGHT_4},
		{QTWO_BRIGHTNESS_LEVEL_BRIGHT_1, QTWO_BRIGHTNESS_LEVEL_BRIGHT_2, QTWO_BRIGHTNESS_LEVEL_BRIGHT_3, QTWO_BRIGHTNESS_LEVEL_BRIGHT_4},
		{QTWO_BRIGHTNESS_LEVEL_BRIGHT_1, QTWO_BRIGHTNESS_LEVEL_BRIGHT_2, QTWO_BRIGHTNESS_LEVEL_BRIGHT_3, QTWO_BRIGHTNESS_LEVEL_BRIGHT_4},
		{QTWO_BRIGHTNESS_LEVEL_BRIGHT_1, QTWO_BRIGHTNESS_LEVEL_BRIGHT_2, QTWO_BRIGHTNESS_LEVEL_BRIGHT_3, QTWO_BRIGHTNESS_LEVEL_BRIGHT_4},
		/* dark blue */
		{(uint8_t)(2.50 * QTWO_BRIGHTNESS_LEVEL_BRIGHT_1), (uint8_t)(1.50 * QTWO_BRIGHTNESS_LEVEL_BRIGHT_2),
				(uint8_t)(1.25 * QTWO_BRIGHTNESS_LEVEL_BRIGHT_3), (uint8_t)(1.25 * QTWO_BRIGHTNESS_LEVEL_BRIGHT_4)}
};

const uint8_t digitSeconds[DIGIT_SIZE_LIN * DIGIT_SIZE_COL * 10] PROGMEM =
{
		/* digits from 0 to 9 */
		0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1,	1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0,
		0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
		1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
		0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
};

static const uint8_t edgesConfig[4] = {3, 2, 1, 4};

static uint8_t matrix[QTWO_LINE_NB][QTWO_COL_NB];
static uint8_t edges[4];
static uint8_t brightnessCurr, selectedColor, currentColor, timeTransition, brightnessTransition;

static RGB_Color_t QtwoColor, offOnColor, onOffColor;
static uint8_t selectedColor_EEPROM EEMEM = 0;
static uint8_t currentColor_EEPROM EEMEM = 0;

static uint8_t clockHoursPrev, clockHoursColorPrev, clockMinPrev, clockSecPrev;

static const uint8_t (*currentBrightnessTable)[QTWO_BRIGHTNESS_NB];
static uint8_t currentBrightnessSetting;
static uint8_t currentBrightnessSetting_EEPROM EEMEM;

static uint8_t selectedLang;
static uint8_t selectedLang_EEPROM EEMEM;

void Qtwo__init (void)
{
	selectedColor = eeprom_read_byte(&selectedColor_EEPROM);

	if (selectedColor < QTWO_COLOR_NB)
	{
		currentColor = selectedColor;
	}
	else
	{
		/* automatic color change */
		selectedColor = QTWO_COLOR_NB;

		if (eeprom_read_byte(&currentColor_EEPROM) < QTWO_COLOR_NB)
		{
			currentColor = eeprom_read_byte(&currentColor_EEPROM);
		}
		else
		{
			currentColor = QTWO_COLOR_NB - 1;
		}
	}

	/* brightness level */
	if ((eeprom_read_byte(&currentBrightnessSetting_EEPROM)) == QTWO_BRIGHTNESS_HIGH)
	{
		currentBrightnessSetting = QTWO_BRIGHTNESS_HIGH;
		currentBrightnessTable = brightnessLevels_high;
	}
	else
	{
		currentBrightnessSetting = QTWO_BRIGHTNESS_LOW;
		currentBrightnessTable = brightnessLevels;
	}

#if (QLOCKTWO_LANG == QLOCKTWO_LANG_DE)
	selectedLang = QTWO_LANG_DE;
#elif (QLOCKTWO_LANG == QLOCKTWO_LANG_DE_SUED)
	selectedLang = QTWO_LANG_DE_SUED;
#elif (selectedLang == QTWO_LANG_FR)
	selectedLang = QTWO_LANG_FR;
#elif (QLOCKTWO_LANG == QLOCKTWO_LANG_EN)
	selectedLang = QTWO_LANG_EN;
#else /* multi */
	if ((eeprom_read_byte(&selectedLang_EEPROM)) < QTWO_LANG_NB)
	{
		selectedLang = eeprom_read_byte(&selectedLang_EEPROM);
	}
	else
	{
		selectedLang = QTWO_LANG_DE_SUED;
	}
#endif
}


static void Qtwo__eepromStorage (void)
{
	eeprom_update_byte(&selectedColor_EEPROM, selectedColor);
	eeprom_update_byte(&currentColor_EEPROM, currentColor);
	eeprom_update_byte(&currentBrightnessSetting_EEPROM, currentBrightnessSetting);
	eeprom_update_byte(&selectedLang_EEPROM, selectedLang);
}


static void Qtwo__incHours (void)
{
	Clock__incHours();
}


static void Qtwo__incMinutes (void)
{
	Clock__incMinutes();
}


static void Qtwo__decHours (void)
{
	Clock__decHours();
}


static void Qtwo__decMinutes (void)
{
	Clock__decMinutes();
}


void Qtwo__setNextLang (void)
{
	if (selectedLang < (QTWO_LANG_NB - 1))
	{
		selectedLang++;

		if (selectedLang == QTWO_LANG_EN)
		{
			/* not suported for now */
			Qtwo__setNextLang();
		}
	}
	else
	{
		selectedLang = 0;
	}

	Qtwo__eepromStorage();
}


static void Qtwo__checkButtons (void)
{
	static uint8_t timerColorButton = TIMER_COLOR_BUTTON;

	if ((!timeTransition) && (!brightnessTransition))
	{
		if (Buttons__isPressedOnce(&buttonFunc1))
		{
			if (selectedColor < (QTWO_COLOR_NB - 1))
			{
				selectedColor++;
			}
			else
			{
				selectedColor = 0;
			}

			Qtwo__eepromStorage();
		}

		if (Buttons__isPressed(&buttonFunc1))
		{
			if (timerColorButton > 0)
			{
				timerColorButton--;
			}
			else
			{
				timerColorButton = TIMER_COLOR_BUTTON;
				selectedColor = QTWO_COLOR_NB;
				currentColor = QTWO_COLOR_NB - 1;
				Qtwo__modeTransition();
				Qtwo__eepromStorage();
			}
		}
		else
		{
			timerColorButton = TIMER_COLOR_BUTTON;
		}

		if (Buttons__isPressedOnce(&buttonFunc2))
		{
			if (currentBrightnessSetting == QTWO_BRIGHTNESS_LOW)
			{
				currentBrightnessSetting = QTWO_BRIGHTNESS_HIGH;
				currentBrightnessTable = brightnessLevels_high;
			}
			else
			{
				currentBrightnessSetting = QTWO_BRIGHTNESS_LOW;
				currentBrightnessTable = brightnessLevels;
			}

			Qtwo__eepromStorage();
		}

		if (Buttons__isPressedOnce(&buttonFunc3))
		{
			Modes__setMode(MODE__TIME_SETUP);
		}
	}
}


static void Qtwo__checkButtonsSetup (void)
{
	if (	(Buttons__isPressedOnce(&buttonUp))
#if (BUTTONS_IRMP != BUTTONS_IRMP_USED)
		|| 	(Buttons__isPressedOnce(&buttonFunc1)) /* for compatibility with old projects */
#endif
	)
	{
		Qtwo__incHours();
	}

	if (Buttons__isPressedOnce(&buttonDown))
	{
		Qtwo__decHours();
	}

	if (	(Buttons__isPressedOnce(&buttonRight))
#if (BUTTONS_IRMP != BUTTONS_IRMP_USED)
		|| 	(Buttons__isPressedOnce(&buttonFunc2)) /* for compatibility with old projects */
#endif
	)
	{
		Qtwo__incMinutes();
	}

	if (Buttons__isPressedOnce(&buttonLeft))
	{
		Qtwo__decMinutes();
	}

	if (Buttons__isPressedOnce(&buttonFunc3))
	{
		Modes__setMode(MODE__QLOCKTWO);
	}
}


static void Qtwo__checkButtonsSeconds (void)
{
	static uint8_t timerColorButton = TIMER_COLOR_BUTTON;

	if ((Buttons__isPressedOnce(&buttonFunc1)) && (!timeTransition) && (!brightnessTransition))
	{
		if (Buttons__isPressedOnce(&buttonFunc1))
		{
			if (selectedColor < (QTWO_COLOR_NB - 1))
			{
				selectedColor++;
			}
			else
			{
				selectedColor = 0;
			}

			Qtwo__eepromStorage();
		}

		if (Buttons__isPressed(&buttonFunc1))
		{
			if (timerColorButton > 0)
			{
				timerColorButton--;
			}
			else
			{
				/* TODO: currently not working because of timeTransition */
				timerColorButton = TIMER_COLOR_BUTTON;
				selectedColor = QTWO_COLOR_NB;
				currentColor = QTWO_COLOR_NB - 1;
				Qtwo__modeTransition();
				Qtwo__eepromStorage();
			}
		}
		else
		{
			timerColorButton = TIMER_COLOR_BUTTON;
		}
	}

	if ((Buttons__isPressedOnce(&buttonFunc2)) && (!timeTransition) && (!brightnessTransition))
	{
		if (currentBrightnessSetting == QTWO_BRIGHTNESS_LOW)
		{
			currentBrightnessSetting = QTWO_BRIGHTNESS_HIGH;
			currentBrightnessTable = brightnessLevels_high;
		}
		else
		{
			currentBrightnessSetting = QTWO_BRIGHTNESS_LOW;
			currentBrightnessTable = brightnessLevels;
		}

		Qtwo__eepromStorage();
	}
}


static void Qtwo__setBrightness (uint8_t stateTransition)
{
	uint16_t adcOutput = ADC__readValue(LDR_ADC_PIN);
	static uint8_t brightnessTar = 0;

	switch (stateLDR)
	{
		case STATE_1:
		{
			brightnessTransition = FALSE;
			brightnessCurr = 0;

			if ((adcOutput) > ((ldrLevels[brightnessCurr + 1]) + QTWO_LDR_HYST))
			{
				stateLDR = STATE_1_TO_2;
			}

			QtwoColor = LEDMatrix__getRGBColorFromComponents(	(currentBrightnessTable[currentColor][brightnessCurr]) * colors[currentColor * 3],
					(currentBrightnessTable[currentColor][brightnessCurr]) * colors[(currentColor * 3) + 1],
					(currentBrightnessTable[currentColor][brightnessCurr]) * colors[(currentColor * 3) + 2]	);
			break;
		}

		case STATE_1_TO_2:
		{
			if (stateTransition)
			{
				brightnessTransition = TRUE;
				brightnessCurr = 0;
				brightnessTar = 1;

				if ((colors[currentColor * 3]) != 0)
				{
					QtwoColor.red++;
				}

				if ((colors[currentColor * 3 + 1]) != 0)
				{
					QtwoColor.green++;
				}

				if ((colors[currentColor * 3 + 2]) != 0)
				{
					QtwoColor.blue++;
				}

				if (		(QtwoColor.red == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3])
						&&	(QtwoColor.green == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 1])
						&&	(QtwoColor.blue == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 2])
				)
				{
					stateLDR = STATE_2;
				}
			}
			else
			{
				stateLDR = STATE_2;
			}

			break;
		}

		case STATE_2:
		{
			brightnessTransition = FALSE;
			brightnessCurr = 1;

			if ((adcOutput) > ((ldrLevels[brightnessCurr + 1]) + QTWO_LDR_HYST))
			{
				stateLDR = STATE_2_TO_3;
			}
			else if ((adcOutput) < (ldrLevels[brightnessCurr]))
			{
				stateLDR = STATE_2_TO_1;
			}

			QtwoColor = LEDMatrix__getRGBColorFromComponents(	(currentBrightnessTable[currentColor][brightnessCurr]) * colors[currentColor * 3],
					(currentBrightnessTable[currentColor][brightnessCurr]) * colors[(currentColor * 3) + 1],
					(currentBrightnessTable[currentColor][brightnessCurr]) * colors[(currentColor * 3) + 2]	);
			break;
		}

		case STATE_2_TO_3:
		{
			if (stateTransition)
			{
				brightnessTransition = TRUE;
				brightnessCurr = 1;
				brightnessTar = 2;

				if ((colors[currentColor * 3]) != 0)
				{
					QtwoColor.red++;
				}

				if ((colors[currentColor * 3 + 1]) != 0)
				{
					QtwoColor.green++;
				}

				if ((colors[currentColor * 3 + 2]) != 0)
				{
					QtwoColor.blue++;
				}


				if (		(QtwoColor.red == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3])
						&&	(QtwoColor.green == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 1])
						&&	(QtwoColor.blue == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 2])
				)
				{
					stateLDR = STATE_3;
				}
			}
			else
			{
				stateLDR = STATE_3;
			}

			break;
		}

		case STATE_3:
		{
			brightnessTransition = FALSE;
			brightnessCurr = 2;

			if ((adcOutput) > ((ldrLevels[brightnessCurr + 1]) + QTWO_LDR_HYST))
			{
				stateLDR = STATE_3_TO_4;
			}
			else if ((adcOutput) < (ldrLevels[brightnessCurr]))
			{
				stateLDR = STATE_3_TO_2;
			}

			QtwoColor = LEDMatrix__getRGBColorFromComponents(	(currentBrightnessTable[currentColor][brightnessCurr]) * colors[currentColor * 3],
					(currentBrightnessTable[currentColor][brightnessCurr]) * colors[(currentColor * 3) + 1],
					(currentBrightnessTable[currentColor][brightnessCurr]) * colors[(currentColor * 3) + 2]	);
			break;
		}

		case STATE_3_TO_4:
		{
			if (stateTransition)
			{
				brightnessTransition = TRUE;
				brightnessCurr = 2;
				brightnessTar = 3;

				if ((colors[currentColor * 3]) != 0)
				{
					QtwoColor.red++;
				}

				if ((colors[currentColor * 3 + 1]) != 0)
				{
					QtwoColor.green++;
				}

				if ((colors[currentColor * 3 + 2]) != 0)
				{
					QtwoColor.blue++;
				}


				if (		(QtwoColor.red == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3])
						&&	(QtwoColor.green == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 1])
						&&	(QtwoColor.blue == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 2])
				)
				{
					stateLDR = STATE_4;
				}
			}
			else
			{
				stateLDR = STATE_4;
			}
			break;
		}

		case STATE_4:
		{
			brightnessTransition = FALSE;
			brightnessCurr = 3;

			if ((adcOutput) < (ldrLevels[brightnessCurr]))
			{
				stateLDR = STATE_4_TO_3;
			}

			QtwoColor = LEDMatrix__getRGBColorFromComponents(	(currentBrightnessTable[currentColor][brightnessCurr]) * colors[currentColor * 3],
					(currentBrightnessTable[currentColor][brightnessCurr]) * colors[(currentColor * 3) + 1],
					(currentBrightnessTable[currentColor][brightnessCurr]) * colors[(currentColor * 3) + 2]	);
			break;
		}

		case STATE_4_TO_3:
		{
			if (stateTransition)
			{
				brightnessTransition = TRUE;
				brightnessCurr = 3;
				brightnessTar = 2;

				if ((colors[currentColor * 3]) != 0)
				{
					QtwoColor.red--;
				}

				if ((colors[currentColor * 3 + 1]) != 0)
				{
					QtwoColor.green--;
				}

				if ((colors[currentColor * 3 + 2]) != 0)
				{
					QtwoColor.blue--;
				}


				if (		(QtwoColor.red == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3])
						&&	(QtwoColor.green == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 1])
						&&	(QtwoColor.blue == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 2])
				)
				{
					stateLDR = STATE_3;
				}
			}
			else
			{
				stateLDR = STATE_3;
			}

			break;
		}

		case STATE_3_TO_2:
		{
			if (stateTransition)
			{
				brightnessTransition = TRUE;
				brightnessCurr = 2;
				brightnessTar = 1;

				if ((colors[currentColor * 3]) != 0)
				{
					QtwoColor.red--;
				}

				if ((colors[currentColor * 3 + 1]) != 0)
				{
					QtwoColor.green--;
				}

				if ((colors[currentColor * 3 + 2]) != 0)
				{
					QtwoColor.blue--;
				}

				if (		(QtwoColor.red == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3])
						&&	(QtwoColor.green == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 1])
						&&	(QtwoColor.blue == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 2])
				)
				{
					stateLDR = STATE_2;
				}
			}
			else
			{
				stateLDR = STATE_2;
			}

			break;
		}

		case STATE_2_TO_1:
		{
			if (stateTransition)
			{
				brightnessTransition = TRUE;
				brightnessCurr = 1;
				brightnessTar = 0;

				if ((colors[currentColor * 3]) != 0)
				{
					QtwoColor.red--;
				}

				if ((colors[currentColor * 3 + 1]) != 0)
				{
					QtwoColor.green--;
				}

				if ((colors[currentColor * 3 + 2]) != 0)
				{
					QtwoColor.blue--;
				}


				if (		(QtwoColor.red == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3])
						||	(QtwoColor.green == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 1])
						||	(QtwoColor.blue == (currentBrightnessTable[currentColor][brightnessTar]) * colors[currentColor * 3 + 2])
				)
				{
					stateLDR = STATE_1;
				}
			}
			else
			{
				stateLDR = STATE_1;
			}

			break;
		}
	}
}


static void Qtwo__setCellActive (uint8_t line, uint8_t col)
{
	if (matrix[line][col] == OFF)
	{
		matrix[line][col] = OFF_TO_ON;
	}

	if (matrix[line][col] == ON_TO_OFF)
	{
		matrix[line][col] = ON;
	}
}


static void Qtwo__setEdgeActive (uint8_t edge)
{
	if (edges[edge] == OFF)
	{
		edges[edge] = OFF_TO_ON;
	}

	if (edges[edge] == ON_TO_OFF)
	{
		edges[edge] = ON;
	}
}


static void Qtwo__getLastVisibility (void)
{
	uint8_t linIt, colIt;

	for (linIt = 0; linIt < QTWO_LINE_NB; linIt++)
	{
		for (colIt = 0; colIt < QTWO_COL_NB; colIt++)
		{
			if (matrix[linIt][colIt] == ON)
			{
				matrix[linIt][colIt] = ON_TO_OFF;
			}
		}
	}

	for (linIt = 0; linIt < 4; linIt++)
	{
		if (edges[linIt] == ON)
		{
			edges[linIt] = ON_TO_OFF;
		}
	}
}


static void Qtwo__updateVisibility (uint8_t timer)
{
	uint8_t linIt, colIt;
	uint8_t allColorsReady = TRUE;
	uint8_t colorStep = (currentBrightnessTable[currentColor][brightnessCurr] / timer) | 1;

	static uint8_t colorTransTimer = 0;

	if (colorTransTimer > 0)
	{
		colorTransTimer--;
		allColorsReady = FALSE;
	}
	else
	{
		if (QtwoColor.red != 0)
		{
			if ((offOnColor.red + colorStep) < QtwoColor.red)
			{
				offOnColor.red = offOnColor.red + colorStep;
				allColorsReady = FALSE;
			}
			else
			{
				offOnColor.red = QtwoColor.red;
			}
		}

		if (QtwoColor.green != 0)
		{
			if ((offOnColor.green + colorStep) < QtwoColor.green)
			{
				offOnColor.green = offOnColor.green + colorStep;
				allColorsReady = FALSE;
			}
			else
			{
				offOnColor.green = QtwoColor.green;
			}
		}

		if (QtwoColor.blue != 0)
		{
			if ((offOnColor.blue + colorStep) < QtwoColor.blue)
			{
				offOnColor.blue = offOnColor.blue + colorStep;
				allColorsReady = FALSE;
			}
			else
			{
				offOnColor.blue = QtwoColor.blue;
			}
		}

		if (onOffColor.red - colorStep > 0)
		{
			onOffColor.red = onOffColor.red - colorStep;
			allColorsReady = FALSE;
		}
		else
		{
			onOffColor.red = 0;
		}

		if (onOffColor.green - colorStep > 0)
		{
			onOffColor.green = onOffColor.green - colorStep;
			allColorsReady = FALSE;
		}
		else
		{
			onOffColor.green = 0;
		}

		if (onOffColor.blue - colorStep > 0)
		{
			onOffColor.blue = onOffColor.blue - colorStep;
			allColorsReady = FALSE;
		}
		else
		{
			onOffColor.blue = 0;
		}

		colorTransTimer = (timer / currentBrightnessTable[currentColor][brightnessCurr]);
	}

	if (allColorsReady == TRUE)
	{
		timeTransition = FALSE;

		for (linIt = 0; linIt < QTWO_LINE_NB; linIt++)
		{
			for (colIt = 0; colIt < QTWO_COL_NB; colIt++)
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

		for (linIt = 0; linIt < 4; linIt++)
		{
			if (edges[linIt] == ON_TO_OFF)
			{
				edges[linIt] = OFF;
			}
			else if (edges[linIt] == OFF_TO_ON)
			{
				edges[linIt] = ON;
			}
		}
	}
}


static void Qtwo__startTransition (void)
{
	timeTransition = TRUE;

	offOnColor.red = 0;
	offOnColor.green = 0;
	offOnColor.blue = 0;
	onOffColor.red = QtwoColor.red;
	onOffColor.green = QtwoColor.green;
	onOffColor.blue = QtwoColor.blue;
}


static void Qtwo__updateMatrix()
{
	uint8_t matrixHours = Clock__getHours();
	uint8_t matrixMinutes = Clock__getMinutes() / 5;
	uint8_t matrixEdges = Clock__getMinutes() % 5;
	uint8_t colIt;

	uint8_t setNACH = FALSE;
	uint8_t setVOR = FALSE;
	uint8_t setMOINS = FALSE;
	uint8_t setHEURES = TRUE;

	if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
	{
		/* ES IST */
		for (colIt = 0; colIt <= 5; colIt++)
		{
			if (colIt != 2)
			{
				Qtwo__setCellActive(0, colIt);
			}
		}
	}

	if (selectedLang == QTWO_LANG_FR)
	{
		/* IL EST */
		for (colIt = 0; colIt <= 5; colIt++)
		{
			if (colIt != 2)
			{
				Qtwo__setCellActive(0, colIt);
			}
		}
	}

	switch (matrixMinutes)
	{
		case 0: /* HH:00 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* UHR */
				for (colIt = 8; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(9, colIt);
				}
			}

			break;
		}

		case 1: /* HH:05 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* FÜNF NACH HH */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(0, colIt);
				}

				setNACH = TRUE;
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* CINQ */
				for (colIt = 6; colIt <= 9; colIt++)
				{
					Qtwo__setCellActive(8, colIt);
				}
			}

			break;
		}

		case 2: /* HH:10 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* ZEHN NACH HH */
				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(1, colIt);
				}
				setNACH = TRUE;
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* DIX */
				for (colIt = 8; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(6, colIt);
				}
			}

			break;
		}

		case 3: /* HH:15 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* VIERTEL */
				for (colIt = 4; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(2, colIt);
				}

				if (selectedLang == QTWO_LANG_DE_SUED)
				{
					/* HH+1 */
					matrixHours++;
				}
				else
				{
					/* NACH */
					setNACH = TRUE;
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* ET QUART */
				for (colIt = 0; colIt <= 7; colIt++)
				{
					if (colIt != 2)
					{
						Qtwo__setCellActive(7, colIt);
					}
				}
			}

			break;
		}

		case 4: /* HH:20 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* ZWANZIG NACH HH */
				for (colIt = 4; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(1, colIt);
				}
				setNACH = TRUE;
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* VINGT */
				for (colIt = 0; colIt <= 4; colIt++)
				{
					Qtwo__setCellActive(8, colIt);
				}
			}

			break;
		}

		case 5: /* HH:25 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* FÜNF VOR HALB HH+1 */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(0, colIt);
				}

				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(4, colIt);
				}

				setVOR = TRUE;
				matrixHours++;
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* VINGT-CINQ */
				for (colIt = 0; colIt <= 9; colIt++)
				{
					Qtwo__setCellActive(8, colIt);
				}
			}

			break;
		}

		case 6: /* HH:30 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* HALB HH+1 */
				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(4, colIt);
				}

				matrixHours++;
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* ET DEMI */
				for (colIt = 0; colIt <= 6; colIt++)
				{
					if (colIt != 2)
					{
						Qtwo__setCellActive(9, colIt);
					}
				}
			}

			break;
		}

		case 7: /* HH:35 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* FÜNF NACH HALB HH+1 */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(0, colIt);
				}

				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(4, colIt);
				}

				setNACH = TRUE;
				matrixHours++;
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* MOINS VINGT-CINQ */
				for (colIt = 0; colIt <= 9; colIt++)
				{
					Qtwo__setCellActive(8, colIt);
				}

				setMOINS = TRUE;
				matrixHours++;
			}

			break;
		}

		case 8: /* HH:40 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* ZWANZIG VOR HH */
				for (colIt = 4; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(1, colIt);
				}

				setVOR = TRUE;
				matrixHours++;
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* VINGT */
				for (colIt = 0; colIt <= 4; colIt++)
				{
					Qtwo__setCellActive(8, colIt);
				}

				setMOINS = TRUE;
				matrixHours++;
			}

			break;
		}

		case 9: /* HH:45 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				if (selectedLang == QTWO_LANG_DE_SUED)
				{
					/* DREIVIERTEL HH+1 */
					for (colIt = 0; colIt <= 10; colIt++)
					{
						Qtwo__setCellActive(2, colIt);
					}
				}
				else
				{
					/* VIERTEL VOR HH+1 */
					for (colIt = 4; colIt <= 10; colIt++)
					{
						Qtwo__setCellActive(2, colIt);
					}

					setVOR = TRUE;
				}

				matrixHours++;
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* MOINS LE QUART */
				for (colIt = 6; colIt <= 7; colIt++)
				{
					Qtwo__setCellActive(6, colIt);
				}

				for (colIt = 3; colIt <= 7; colIt++)
				{
					Qtwo__setCellActive(7, colIt);
				}

				setMOINS = TRUE;
				matrixHours++;
			}

			break;
		}

		case 10: /* HH:50 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* ZEHN VOR HH+1 */
				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(1, colIt);
				}

				setVOR = TRUE;
				matrixHours++;
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* MOINS DIX */
				for (colIt = 8; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(6, colIt);
				}

				setMOINS = TRUE;
				matrixHours++;
			}

			break;
		}

		case 11: /* HH:55 */
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* FÜNF VOR HH+1 */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(0, colIt);
				}

				setVOR = TRUE;
				matrixHours++;
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* MOINS CINQ */
				for (colIt = 6; colIt <= 9; colIt++)
				{
					Qtwo__setCellActive(8, colIt);
				}

				setMOINS = TRUE;
				matrixHours++;
			}

			break;
		}

		default:
		{
			break;
		}
	}

	matrixHours = matrixHours % 24; /* avoid overflow */

	if (matrixHours > 12)
	{
		matrixHours -= 12;
	}

	switch (matrixHours)
	{
		case 0:
		{
			if (selectedLang == QTWO_LANG_FR)
			{
				/* MINUIT */
				for (colIt = 5; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(4, colIt);
				}

				setHEURES = FALSE;
			}

			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* ZWÖLF */
				for (colIt = 6; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(8, colIt);
				}
			}

			break;
		}

		case 12:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* ZWÖLF */
				for (colIt = 6; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(8, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* MIDI */
				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(4, colIt);
				}

				setHEURES = FALSE;
			}

			break;
		}

		case 1:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				if (matrixMinutes == 0)
				{
					/* EIN */
					for (colIt = 0; colIt <= 2; colIt++)
					{
						Qtwo__setCellActive(5, colIt);
					}
				}
				else
				{
					/* EINS */
					for (colIt = 0; colIt <= 3; colIt++)
					{
						Qtwo__setCellActive(5, colIt);
					}
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* UNE */
				for (colIt = 4; colIt <= 6; colIt++)
				{
					Qtwo__setCellActive(2, colIt);
				}
			}

			break;
		}

		case 2:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* ZWEI */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(5, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* DEUX */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(0, colIt);
				}
			}

			break;
		}

		case 3:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* DREI */
				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(6, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* TROIS */
				for (colIt = 6; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(1, colIt);
				}
			}

			break;
		}

		case 4:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* VIER */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(6, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* QUATRE */
				for (colIt = 0; colIt <= 5; colIt++)
				{
					Qtwo__setCellActive(1, colIt);
				}
			}

			break;
		}

		case 5:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* FÜNF */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(4, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* CINQ */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(3, colIt);
				}
			}

			break;
		}

		case 6:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* SECHS */
				for (colIt = 0; colIt <= 4; colIt++)
				{
					Qtwo__setCellActive(7, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* SIX */
				for (colIt = 4; colIt <= 6; colIt++)
				{
					Qtwo__setCellActive(3, colIt);
				}
			}

			break;
		}
		case 7:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* SIEBEN */
				for (colIt = 0; colIt <= 5; colIt++)
				{
					Qtwo__setCellActive(8, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* MIDI */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(2, colIt);
				}
			}

			break;
		}
		case 8:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* ACHT */
				for (colIt = 7; colIt <= 10; colIt++)
				{
					Qtwo__setCellActive(7, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* HUIT */
				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(3, colIt);
				}
			}

			break;
		}
		case 9:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* NEUN */
				for (colIt = 3; colIt <= 6; colIt++)
				{
					Qtwo__setCellActive(9, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* NEUF */
				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(2, colIt);
				}
			}

			break;
		}
		case 10:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* ZEHN */
				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(9, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* DIX */
				for (colIt = 2; colIt <= 4; colIt++)
				{
					Qtwo__setCellActive(4, colIt);
				}
			}

			break;
		}
		case 11:
		{
			if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
			{
				/* ELF */
				for (colIt = 5; colIt <= 7; colIt++)
				{
					Qtwo__setCellActive(4, colIt);
				}
			}

			if (selectedLang == QTWO_LANG_FR)
			{
				/* ONZE */
				for (colIt = 0; colIt <= 3; colIt++)
				{
					Qtwo__setCellActive(5, colIt);
				}
			}

			break;
		}

		default:
		{
			break;
		}
	}

	switch (matrixEdges)
	{
		case 0:
		{
			break;
		}
		case 1:
		{
			Qtwo__setEdgeActive(0);
			break;
		}

		case 2:
		{
			Qtwo__setEdgeActive(0);
			Qtwo__setEdgeActive(1);
			break;
		}

		case 3:
		{
			Qtwo__setEdgeActive(0);
			Qtwo__setEdgeActive(1);
			Qtwo__setEdgeActive(2);
			break;
		}

		case 4:
		{
			Qtwo__setEdgeActive(0);
			Qtwo__setEdgeActive(1);
			Qtwo__setEdgeActive(2);
			Qtwo__setEdgeActive(3);
			break;
		}

		default:
		{
			break;
		}
	}

	if ((selectedLang == QTWO_LANG_DE) || (selectedLang == QTWO_LANG_DE_SUED))
	{
		if (setNACH)
		{
			for (colIt = 7; colIt <= 10; colIt++)
			{
				Qtwo__setCellActive(3, colIt);
			}
		}

		if (setVOR)
		{
			for (colIt = 0; colIt <= 2; colIt++)
			{
				Qtwo__setCellActive(3, colIt);
			}
		}
	}

	if (selectedLang == QTWO_LANG_FR)
	{
		if (setHEURES)
		{
			for (colIt = 5; colIt <= 9; colIt++)
			{
				Qtwo__setCellActive(5, colIt);
			}

			if (matrixHours != 1)
			{
				Qtwo__setCellActive(5, 10);
			}
		}

		if (setMOINS)
		{
			for (colIt = 0; colIt <= 4; colIt++)
			{
				Qtwo__setCellActive(6, colIt);
			}
		}
	}
}


static void Qtwo__updateSeconds (void)
{
	uint8_t clockSeconds = Clock__getSeconds();
	uint8_t digitIt;

	for (digitIt = 0; digitIt < DIGIT_SIZE_COL * DIGIT_SIZE_LIN; digitIt++)
	{
		if (pgm_read_byte(&digitSeconds[(clockSeconds / 10) * (DIGIT_SIZE_COL * DIGIT_SIZE_LIN)] + digitIt) == 1)
		{
			Qtwo__setCellActive(1 + (digitIt / DIGIT_SIZE_COL),  digitIt % DIGIT_SIZE_COL);
		}

		if (pgm_read_byte(&digitSeconds[(clockSeconds % 10) * (DIGIT_SIZE_COL * DIGIT_SIZE_LIN)] + digitIt) == 1)
		{
			Qtwo__setCellActive(1 + (digitIt / DIGIT_SIZE_COL),  6 + (digitIt % DIGIT_SIZE_COL));
		}
	}
}


void Qtwo__updateLeds (void)
{
	uint8_t linIt, colIt, edgeIt;

	for (colIt = 0; colIt < QTWO_COL_NB; colIt++)
	{
		for (linIt = 0; linIt < QTWO_LINE_NB; linIt++)
		{
			if (matrix[linIt][colIt] == ON)
			{
				LEDMatrix__setRGBColor(linIt + 1, colIt + 1, QtwoColor);
			}
			else if (matrix[linIt][colIt] == ON_TO_OFF)
			{
				LEDMatrix__setRGBColor(linIt + 1, colIt + 1, onOffColor);
			}
			else if (matrix[linIt][colIt] == OFF_TO_ON)
			{
				LEDMatrix__setRGBColor(linIt + 1, colIt + 1, offOnColor);
			}
		}
	}

	for (edgeIt = 0; edgeIt < 4; edgeIt++)
	{
		if (edges[edgeIt] == ON)
		{
			LEDMatrix__setRGBColor(QTWO_LINE_NB, QTWO_COL_NB + edgesConfig[edgeIt], QtwoColor);
		}
		else if (edges[edgeIt] == ON_TO_OFF)
		{
			LEDMatrix__setRGBColor(QTWO_LINE_NB, QTWO_COL_NB + edgesConfig[edgeIt], onOffColor);
		}
		else if (edges[edgeIt] == OFF_TO_ON)
		{
			LEDMatrix__setRGBColor(QTWO_LINE_NB, QTWO_COL_NB + edgesConfig[edgeIt], offOnColor);
		}
	}
#if 0
	/* TEST BRIGHTNESS */
	for (colIt = 0; colIt < QTWO_COL_NB; colIt++)
	{
		if (brightnessCurr >= colIt)
		{
			LEDMatrix__setRGBColor(6, colIt + 1, QtwoColor);
		}
	}
#endif
}


static void Qtwo__updateColor (void)
{
	if (selectedColor < QTWO_COLOR_NB)
	{
		currentColor = selectedColor;
	}
	else
	{
		if ((Clock__getHours() != clockHoursColorPrev) && ((Clock__getHours() % QTWO_COLOR_CHANGE_HOURS) == 0))
		{
			clockHoursColorPrev = Clock__getHours();

			if (selectedColor == QTWO_COLOR_NB)
			{
				if (currentColor < (QTWO_COLOR_NB - 1))
				{
					currentColor++;
				}
				else
				{
					currentColor = 0;
				}

				/* store new color in EEPROM */
				Qtwo__eepromStorage();
			}
		}
	}
}


void Qtwo__main_x10 (void)
{
	Qtwo__checkButtons();
	Qtwo__updateColor();

	if ((!timeTransition) && (!brightnessTransition))
	{
		if ((clockHoursPrev != Clock__getHours()) || (clockMinPrev != Clock__getMinutes()))
		{
			Qtwo__getLastVisibility();
			Qtwo__updateMatrix();
			clockHoursPrev = Clock__getHours();
			clockMinPrev = Clock__getMinutes();
			Qtwo__startTransition();
		}
	}

	Qtwo__updateVisibility(QTWO_TRANSITION_TIMER);
	Qtwo__setBrightness(TRUE);
	Qtwo__updateLeds();
}


void Qtwo__seconds_x10 (void)
{
	Qtwo__updateColor();

	if (!timeTransition)
	{
		if (clockSecPrev != Clock__getSeconds())
		{
			Qtwo__getLastVisibility();
			Qtwo__updateSeconds();
			clockSecPrev = Clock__getSeconds();
			Qtwo__startTransition();
		}

		Qtwo__setBrightness(FALSE);
	}

	Qtwo__updateVisibility(QTWO_TRANSITION_TIMER_SEC);
	Qtwo__updateLeds();
	Qtwo__checkButtonsSeconds();
}


void Qtwo__timeSetup_x10 (void)
{
	static uint8_t QtwoSetupDisplayOn = FALSE;
	static uint8_t QtwoSetupTimer = 0;

	Qtwo__checkButtonsSetup();
	Qtwo__updateColor();

	if (		(!timeTransition)
			&& 	((clockHoursPrev != Clock__getHours()) || (clockMinPrev != Clock__getMinutes()))
	)
	{
		Qtwo__getLastVisibility();
		Qtwo__updateMatrix();
		Qtwo__startTransition();
		clockHoursPrev = Clock__getHours();
		clockMinPrev = Clock__getMinutes();
	}

	Qtwo__updateVisibility(QTWO_TRANSITION_TIMER_SETUP);
	Qtwo__setBrightness(!timeTransition);

	if (QtwoSetupTimer > 0)
	{
		QtwoSetupTimer--;

		if (QtwoSetupDisplayOn == TRUE)
		{
			Qtwo__updateLeds();
		}
	}
	else
	{
		QtwoSetupTimer = QTWO_SETUP_TIMER;

		if (QtwoSetupDisplayOn == FALSE)
		{
			QtwoSetupDisplayOn = TRUE;
		}
		else
		{
			QtwoSetupDisplayOn = FALSE;
		}
	}
}


void Qtwo__modeTransition (void)
{
	uint8_t linIt, colIt, edgeIt;

	for (colIt = 0; colIt < QTWO_COL_NB; colIt++)
	{
		for (linIt = 0; linIt < QTWO_LINE_NB; linIt++)
		{
			matrix[linIt][colIt] = OFF;
		}
	}

	for (edgeIt = 0; edgeIt < 4; edgeIt++)
	{
		edges[edgeIt] = OFF;
	}

	clockHoursPrev = 0xFF;
	clockMinPrev = 0xFF;
	clockSecPrev = 0xFF;
	clockHoursColorPrev = Clock__getHours();
}
