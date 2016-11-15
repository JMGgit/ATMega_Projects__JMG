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
#define QTWO_TRANSITION_TIMER_SEC		30

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

typedef enum
{
	QTWO_STATE_IDLE = 0,
	QTWO_STATE_TIME_TRANSITION,
	QTWO_STATE_BRIGHTNESS_TRANSITION,
	QTWO_STATE_SWITCH_BRIGHTNESS_LEVEL,
	QTWO_STATE_NEW_COLOR,
	QTWO_STATE_SWITCH_AUTO_COLOR,
	QTWO_STATE_SWITCH_MANUAL_COLOR,
	QTWO_STATE_SET_NEXT_COLOR
} StateQtwo_N;

typedef enum
{
	BUTTON_NO_REQUEST = 0,
	BUTTON_REQUEST_NEW_COLOR,
	BUTTON_REQUEST_AUTO_COLOR,
	BUTTON_REQUEST_SWITCH_BRIGHTNESS,
} ButtonRequest_N;

typedef enum
{
	AUTOCOLOR_NO_REQUEST = 0,
	AUTOCOLOR_REQUEST_NEXT,

} AutoColorRequest_N;

typedef enum
{
	BRIGHTNESS_NO_REQUEST = 0,
	BRIGHTNESS_REQUEST_TRANSITION
} BrightnessRequest_N;

typedef enum
{
	TIME_NO_REQUEST = 0,
	TIME_REQUEST_TRANSITION,
} TimeRequest_N;

typedef enum
{
	TIME_TRANSITION_FINISHED = 0,
	TIME_TRANSITION_STARTED
} TimeTransition_N;

static const uint16_t adcLdrLevels[QTWO_BRIGHTNESS_NB] = {0, 200, 350, 600};

typedef enum
{
	LDR_STATE_1 = 0,
	LDR_STATE_1_TO_2,
	LDR_STATE_2,
	LDR_STATE_2_TO_3,
	LDR_STATE_2_TO_1,
	LDR_STATE_3,
	LDR_STATE_3_TO_2,
	LDR_STATE_3_TO_4,
	LDR_STATE_4,
	LDR_STATE_4_TO_3
} LDRState_N;

LDRState_N LDRstate;

static const uint8_t colors[3 * QTWO_COLOR_NB] =
{
		1, 1, 1,
		1, 0, 0,
		1, 1, 0,
		0, 1, 0,
		0, 1, 1,
		1, 0, 1,
		0, 0, 1
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
static uint8_t currentBrightness, modeAutoColor, modeAutoColorPrev, currentColor, targetColor, brightnessRGBLevel, colorTransTimer;

static RGB_Color_t qtwoColor, offOnColor, onOffColor;
static uint8_t modeAutoColor_EEPROM EEMEM = 0;
static uint8_t currentColor_EEPROM EEMEM = 0;

static uint8_t clockHoursPrev, clockHoursColorPrev, clockMinPrev, clockSecPrev;

static const uint8_t (*currentBrightnessTable)[QTWO_BRIGHTNESS_NB];
static uint8_t currentBrightnessSetting;
static uint8_t currentBrightnessSetting_EEPROM EEMEM;

static uint8_t selectedLang;
static uint8_t selectedLang_EEPROM EEMEM;

static StateQtwo_N QtwoState;
static TimeTransition_N timeTransition;

static uint8_t qtwoReInit;


void Qtwo__init (void)
{
	if (eeprom_read_byte(&modeAutoColor_EEPROM) == TRUE)
	{
		modeAutoColor = TRUE;
	}
	else
	{
		/* automatic color change */
		modeAutoColor = FALSE;
		eeprom_update_byte(&modeAutoColor_EEPROM, modeAutoColor);
	}

	if (eeprom_read_byte(&currentColor_EEPROM) < QTWO_COLOR_NB)
	{
		currentColor = eeprom_read_byte(&currentColor_EEPROM);
	}
	else
	{
		currentColor = 0;
		eeprom_update_byte(&currentColor_EEPROM, currentColor);
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
		eeprom_update_byte(&currentBrightnessSetting_EEPROM, currentBrightnessSetting);
	}


	if ((eeprom_read_byte(&selectedLang_EEPROM)) < QTWO_LANG_NB)
	{
		selectedLang = eeprom_read_byte(&selectedLang_EEPROM);
	}
	else
	{
		selectedLang = QTWO_LANG_DE_SUED;
		eeprom_update_byte(&selectedLang_EEPROM, selectedLang);
	}

	/* other states initialized in Qtwo__modeTransition() */
}


static void Qtwo__eepromStorage (void)
{
	eeprom_update_byte(&modeAutoColor_EEPROM, modeAutoColor);
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


uint8_t Qtwo__getCurrentBrightness (void)
{
	return (brightnessRGBLevel);
}


RGB_Color_t Qtwo__getCurrentColor (void)
{
	return qtwoColor;
}


static BrightnessRequest_N Qtwo__checkBrightness (void)
{
	uint16_t adcOutput = ADC__readValue(LDR_ADC_PIN);
	BrightnessRequest_N brighntessRequest = BRIGHTNESS_NO_REQUEST;

	switch (LDRstate)
	{
		case LDR_STATE_1:
		{
			if ((adcOutput) > ((adcLdrLevels[currentBrightness + 1]) + QTWO_LDR_HYST))
			{
				LDRstate = LDR_STATE_1_TO_2;
				brighntessRequest = BRIGHTNESS_REQUEST_TRANSITION;
			}

			break;
		}

		case LDR_STATE_2:
		{
			if ((adcOutput) > ((adcLdrLevels[currentBrightness + 1]) + QTWO_LDR_HYST))
			{
				LDRstate = LDR_STATE_2_TO_3;
				brighntessRequest = BRIGHTNESS_REQUEST_TRANSITION;
			}
			else if ((adcOutput) < (adcLdrLevels[currentBrightness]))
			{
				LDRstate = LDR_STATE_2_TO_1;
				brighntessRequest = BRIGHTNESS_REQUEST_TRANSITION;
			}
			else
			{
				/* nothing to do */
			}

			break;
		}

		case LDR_STATE_3:
		{
			if ((adcOutput) > ((adcLdrLevels[currentBrightness + 1]) + QTWO_LDR_HYST))
			{
				LDRstate = LDR_STATE_3_TO_4;
				brighntessRequest = BRIGHTNESS_REQUEST_TRANSITION;
			}
			else if ((adcOutput) < (adcLdrLevels[currentBrightness]))
			{
				LDRstate = LDR_STATE_3_TO_2;
				brighntessRequest = BRIGHTNESS_REQUEST_TRANSITION;
			}
			else
			{
				/* nothing to do */
			}

			break;
		}

		case LDR_STATE_4:
		{
			if ((adcOutput) < (adcLdrLevels[currentBrightness]))
			{
				LDRstate = LDR_STATE_4_TO_3;
				brighntessRequest = BRIGHTNESS_REQUEST_TRANSITION;
			}
			else
			{
				/* nothing to do */
			}

			break;
		}

		default:
		{
			/* other case handled in function Qtwo__updateBrightness() */
			break;
		}
	}

	return brighntessRequest;
}


static BrightnessRequest_N Qtwo__updateBrightness (QtwoMode_N qtwoMode)
{
	static uint8_t targetBrightness = 0;

	BrightnessRequest_N brighntessRequest = BRIGHTNESS_NO_REQUEST;

	if (qtwoMode != QTWO_MODE_NORMAL)
	{
		if (LDRstate == LDR_STATE_1_TO_2)
		{
			LDRstate = LDR_STATE_2;
		}
		else if (LDRstate == LDR_STATE_2_TO_3)
		{
			LDRstate = LDR_STATE_3;
		}
		else if (LDRstate == LDR_STATE_3_TO_4)
		{
			LDRstate = LDR_STATE_4;
		}
		else if (LDRstate == LDR_STATE_4_TO_3)
		{
			LDRstate = LDR_STATE_3;
		}
		else if (LDRstate == LDR_STATE_3_TO_2)
		{
			LDRstate = LDR_STATE_2;
		}
		else if (LDRstate == LDR_STATE_2_TO_1)
		{
			LDRstate = LDR_STATE_1;
		}
		else
		{
			/* nothing to do */
		}
	}

	switch (LDRstate)
	{
		case LDR_STATE_1:
		{
			currentBrightness = 0;
			targetBrightness = 0;
			brightnessRGBLevel = currentBrightnessTable[currentColor][currentBrightness];

			break;
		}

		case LDR_STATE_2:
		{
			currentBrightness = 1;
			targetBrightness = 1;
			brightnessRGBLevel = currentBrightnessTable[currentColor][currentBrightness];

			break;
		}

		case LDR_STATE_3:
		{
			currentBrightness = 2;
			targetBrightness = 2;
			brightnessRGBLevel = currentBrightnessTable[currentColor][currentBrightness];

			break;
		}

		case LDR_STATE_4:
		{
			currentBrightness = 3;
			targetBrightness = 3;
			brightnessRGBLevel = currentBrightnessTable[currentColor][currentBrightness];

			break;
		}

		case LDR_STATE_1_TO_2:
		{

			currentBrightness = 0;
			targetBrightness = 1;

			brightnessRGBLevel++;

			if 	(brightnessRGBLevel >= (currentBrightnessTable[currentColor][targetBrightness]))
			{
				LDRstate = LDR_STATE_2;
			}

			break;
		}

		case LDR_STATE_2_TO_3:
		{
			currentBrightness = 1;
			targetBrightness = 2;

			brightnessRGBLevel++;

			if 	(brightnessRGBLevel >= (currentBrightnessTable[currentColor][targetBrightness]))
			{
				LDRstate = LDR_STATE_3;
			}

			break;
		}

		case LDR_STATE_3_TO_4:
		{

			currentBrightness = 2;
			targetBrightness = 3;

			brightnessRGBLevel++;

			if 	(brightnessRGBLevel >= (currentBrightnessTable[currentColor][targetBrightness]))
			{
				LDRstate = LDR_STATE_4;
			}


			break;
		}

		case LDR_STATE_4_TO_3:
		{

			currentBrightness = 3;
			targetBrightness = 2;

			brightnessRGBLevel--;

			if 	(brightnessRGBLevel <= (currentBrightnessTable[currentColor][targetBrightness]))
			{
				LDRstate = LDR_STATE_3;
			}

			break;
		}

		case LDR_STATE_3_TO_2:
		{

			currentBrightness = 2;
			targetBrightness = 1;

			brightnessRGBLevel--;

			if 	(brightnessRGBLevel <= (currentBrightnessTable[currentColor][targetBrightness]))
			{
				LDRstate = LDR_STATE_2;
			}

			break;
		}

		case LDR_STATE_2_TO_1:
		{

			currentBrightness = 1;
			targetBrightness = 0;

			brightnessRGBLevel--;

			if 	(brightnessRGBLevel <= (currentBrightnessTable[currentColor][targetBrightness]))
			{
				LDRstate = LDR_STATE_1;
			}

			break;
		}
	}

	if (targetBrightness != currentBrightness)
	{
		brighntessRequest = BRIGHTNESS_REQUEST_TRANSITION;
	}

	return brighntessRequest;
}


void Qtwo__setLineActive (uint8_t line, uint8_t startCol, uint8_t endCol)
{
	uint8_t colIt;

	for (colIt = startCol; colIt <= endCol; colIt ++)
	{
		if (matrix[line][colIt] == OFF)
		{
			matrix[line][colIt] = OFF_TO_ON;
		}

		if (matrix[line][colIt] == ON_TO_OFF)
		{
			matrix[line][colIt] = ON;
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


static void Qtwo__updateTimeMatrix()
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


static void Qtwo__clearMatrix (void)
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
}


static void Qtwo__prepareMatrixForTimeTransition (void)
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


static TimeTransition_N Qtwo__updateTimeTransition (QtwoMode_N qtwoMode)
{
	uint8_t linIt, colIt;
	uint8_t allColorsReady = TRUE;
	static uint8_t colorStep = 0;
	static uint8_t timer = 0;
	static RGB_Color_t qtwoColorForTransition = {0, 0, 0};
	static QtwoMode_N previousQtwoMode;

	if (timeTransition == TIME_TRANSITION_FINISHED)
	{
		if ((qtwoMode == QTWO_MODE_NORMAL) || (qtwoMode == QTWO_MODE_SETUP))
		{
			timer = QTWO_TRANSITION_TIMER;
		}
		else if (qtwoMode == QTWO_MODE_SECONDS)
		{
			if (previousQtwoMode == QTWO_MODE_NORMAL)
			{
				Qtwo__clearMatrix();
			}

			timer = QTWO_TRANSITION_TIMER_SEC;
		}
		else
		{
			/* nothing to do */
			timer = 0;
		}

		colorStep = (uint8_t)((((float)Qtwo__getCurrentBrightness() / (float)timer)) + 0.5);

		if (colorStep == 0)
		{
			colorStep = 1;
		}

		Qtwo__prepareMatrixForTimeTransition();

		qtwoColorForTransition.red = qtwoColor.red;
		qtwoColorForTransition.green = qtwoColor.green;
		qtwoColorForTransition.blue = qtwoColor.blue;

		if ((qtwoMode == QTWO_MODE_NORMAL) || (qtwoMode == QTWO_MODE_SETUP))
		{
			Qtwo__updateTimeMatrix();
		}
		else if (qtwoMode == QTWO_MODE_SECONDS)
		{
			Qtwo__updateSeconds();
		}
		else
		{
			/* nothing to do */
		}

		offOnColor.red = 0;
		offOnColor.green = 0;
		offOnColor.blue = 0;
		onOffColor.red = qtwoColorForTransition.red;
		onOffColor.green = qtwoColorForTransition.green;
		onOffColor.blue = qtwoColorForTransition.blue;

		colorTransTimer = (timer / Qtwo__getCurrentBrightness());

		timeTransition = TIME_TRANSITION_STARTED;
	}

	if (timeTransition == TIME_TRANSITION_STARTED)
	{
		if (colorTransTimer - uC__getTaskIncrement() > 0)
		{
			colorTransTimer = colorTransTimer - uC__getTaskIncrement();
			allColorsReady = FALSE;
		}
		else
		{
			if (qtwoColorForTransition.red != 0)
			{
				if ((offOnColor.red + colorStep) < qtwoColorForTransition.red)
				{
					offOnColor.red = offOnColor.red + colorStep;
					allColorsReady = FALSE;
				}
				else
				{
					offOnColor.red = qtwoColorForTransition.red;
				}
			}

			if (qtwoColorForTransition.green != 0)
			{
				if ((offOnColor.green + colorStep) < qtwoColorForTransition.green)
				{
					offOnColor.green = offOnColor.green + colorStep;
					allColorsReady = FALSE;
				}
				else
				{
					offOnColor.green = qtwoColorForTransition.green;
				}
			}

			if (qtwoColorForTransition.blue != 0)
			{
				if ((offOnColor.blue + colorStep) < qtwoColorForTransition.blue)
				{
					offOnColor.blue = offOnColor.blue + colorStep;
					allColorsReady = FALSE;
				}
				else
				{
					offOnColor.blue = qtwoColorForTransition.blue;
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

			colorTransTimer = (timer / Qtwo__getCurrentBrightness());
		}

		if (allColorsReady == TRUE)
		{
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
					else
					{
						/* nothing to do */
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
				else
				{
					/* nothing to do */
				}
			}

			timeTransition = TIME_TRANSITION_FINISHED;
		}
	}
	else
	{
		/* not defined */
	}

	previousQtwoMode = qtwoMode;

	return timeTransition;
}


static void Qtwo__updateLeds (void)
{
	uint8_t linIt, colIt, edgeIt;

	LEDMatrix__clearMatrix();

	for (colIt = 0; colIt < QTWO_COL_NB; colIt++)
	{
		for (linIt = 0; linIt < QTWO_LINE_NB; linIt++)
		{
			if (matrix[linIt][colIt] == ON)
			{
				LEDMatrix__setRGBColor(linIt + 1, colIt + 1, qtwoColor);
			}
			else if (matrix[linIt][colIt] == ON_TO_OFF)
			{
				LEDMatrix__setRGBColor(linIt + 1, colIt + 1, onOffColor);
			}
			else if (matrix[linIt][colIt] == OFF_TO_ON)
			{
				LEDMatrix__setRGBColor(linIt + 1, colIt + 1, offOnColor);
			}
			else
			{
				/* nothing to do */
			}
		}
	}

	for (edgeIt = 0; edgeIt < 4; edgeIt++)
	{
		if (edges[edgeIt] == ON)
		{
			LEDMatrix__setRGBColor(QTWO_LINE_NB, QTWO_COL_NB + edgesConfig[edgeIt], qtwoColor);
		}
		else if (edges[edgeIt] == ON_TO_OFF)
		{
			LEDMatrix__setRGBColor(QTWO_LINE_NB, QTWO_COL_NB + edgesConfig[edgeIt], onOffColor);
		}
		else if (edges[edgeIt] == OFF_TO_ON)
		{
			LEDMatrix__setRGBColor(QTWO_LINE_NB, QTWO_COL_NB + edgesConfig[edgeIt], offOnColor);
		}
		else
		{
			/* nothing to do */
		}
	}

#if 0 /* TEST BRIGHTNESS */
	for (linIt = 4; linIt < 4 + QTWO_COL_NB; linIt++)
	{
		if (currentBrightness >= (linIt - 4))
		{
			LEDMatrix__setRGBColor(linIt, 7, qtwoColor);
		}
	}
#endif
}


static AutoColorRequest_N Qtwo__checkAutomaticColor (void)
{
	AutoColorRequest_N request = AUTOCOLOR_NO_REQUEST;

	if (modeAutoColor == TRUE)
	{
		if ((Clock__getHours() != clockHoursColorPrev) && ((Clock__getHours() % QTWO_COLOR_CHANGE_HOURS) == 0))
		{
			clockHoursColorPrev = Clock__getHours();
			request = AUTOCOLOR_REQUEST_NEXT;
		}
	}

	return request;
}


static ButtonRequest_N Qtwo__checkButtons (QtwoMode_N qtwoMode)
{
	static uint8_t timerColorButton = TIMER_COLOR_BUTTON;
	ButtonRequest_N request = BUTTON_NO_REQUEST;

	if ((qtwoMode == QTWO_MODE_NORMAL) || (qtwoMode == QTWO_MODE_SECONDS))
	{
		if (Buttons__isPressedOnce(&buttonFunc1))
		{
			request = BUTTON_REQUEST_NEW_COLOR;
		}

		if (Buttons__isPressed(&buttonFunc1))
		{
			if (timerColorButton - uC__getTaskIncrement() > 0)
			{
				timerColorButton = timerColorButton - uC__getTaskIncrement();
			}
			else
			{
				timerColorButton = TIMER_COLOR_BUTTON;
				request = BUTTON_REQUEST_AUTO_COLOR;
			}
		}
		else
		{
			timerColorButton = TIMER_COLOR_BUTTON;
		}

		if (Buttons__isPressedOnce(&buttonFunc2))
		{
			request = BUTTON_REQUEST_SWITCH_BRIGHTNESS;
		}

		if (qtwoMode == QTWO_MODE_NORMAL)
		{
			if (Buttons__isPressedOnce(&buttonFunc3))
			{
				Modes__setMode(MODE__TIME_SETUP, FALSE);
			}
		}
	}
	else if (qtwoMode == QTWO_MODE_SETUP)
	{
		if (Buttons__isPressedOnce(&buttonUp))
		{
			Qtwo__incHours();
		}

		if (Buttons__isPressedOnce(&buttonDown))
		{
			Qtwo__decHours();
		}

		if (Buttons__isPressedOnce(&buttonRight))
		{
			Qtwo__incMinutes();
		}

		if (Buttons__isPressedOnce(&buttonLeft))
		{
			Qtwo__decMinutes();
		}

		if (Buttons__isPressedOnce(&buttonFunc3))
		{
			Modes__setMode(MODE__QLOCKTWO, FALSE);
		}
	}
	else
	{
		/* nothing to do */
	}


	return request;
}


static TimeRequest_N Qtwo__checkNewTime (QtwoMode_N qtwoMode)
{
	TimeRequest_N timeRequest = TIME_NO_REQUEST;

	if ((qtwoMode == QTWO_MODE_NORMAL) || (qtwoMode == QTWO_MODE_SETUP))
	{
		if ((clockHoursPrev != Clock__getHours()) || (clockMinPrev != Clock__getMinutes()))
		{
			timeRequest = TIME_REQUEST_TRANSITION;
			clockHoursPrev = Clock__getHours();
			clockMinPrev = Clock__getMinutes();
		}
	}
	else if (qtwoMode == QTWO_MODE_SECONDS)
	{
		if (clockSecPrev != Clock__getSeconds())
		{
			timeRequest = TIME_REQUEST_TRANSITION;
			clockSecPrev = Clock__getSeconds();
		}
	}
	else
	{
		/* nothing to do */
	}

	return timeRequest;
}


void Qtwo__main_x10 (QtwoMode_N qtwoMode)
{
	ButtonRequest_N buttonRequest = BUTTON_NO_REQUEST;
	AutoColorRequest_N autoColorRequest = AUTOCOLOR_NO_REQUEST;
	BrightnessRequest_N brightnessRequest = BRIGHTNESS_NO_REQUEST;
	TimeRequest_N timeRequest = TIME_NO_REQUEST;

	static uint8_t QtwoSetupDisplayOn = FALSE;
	static uint8_t QtwoSetupTimer = 0;

	Debug__setModeState((uint8_t)QtwoState);

	qtwoColor =	LEDMatrix__getRGBColorFromComponents(
		Qtwo__getCurrentBrightness() * colors[currentColor * 3],
		Qtwo__getCurrentBrightness() * colors[(currentColor * 3) + 1],
		Qtwo__getCurrentBrightness() * colors[(currentColor * 3) + 2]);

	switch (QtwoState)
	{
		case QTWO_STATE_IDLE:
		{
			if (!qtwoReInit)
			{
				timeRequest = Qtwo__checkNewTime(qtwoMode);
			}
			else
			{
				timeRequest = TIME_REQUEST_TRANSITION;
			}

			if (timeRequest == TIME_REQUEST_TRANSITION)
			{
				QtwoState = QTWO_STATE_TIME_TRANSITION;
			}
			else
			{
				buttonRequest = Qtwo__checkButtons(qtwoMode);

				if (buttonRequest == BUTTON_REQUEST_AUTO_COLOR)
				{
					QtwoState = QTWO_STATE_SWITCH_AUTO_COLOR;
				}
				else if (buttonRequest == BUTTON_REQUEST_NEW_COLOR)
				{
					QtwoState = QTWO_STATE_SWITCH_MANUAL_COLOR;
				}
				else if (buttonRequest == BUTTON_REQUEST_SWITCH_BRIGHTNESS)
				{
					QtwoState = QTWO_STATE_SWITCH_BRIGHTNESS_LEVEL;
				}
				else
				{
					autoColorRequest = Qtwo__checkAutomaticColor();

					if (autoColorRequest == AUTOCOLOR_REQUEST_NEXT)
					{
						QtwoState = QTWO_STATE_SET_NEXT_COLOR;
					}
					else
					{
						brightnessRequest = Qtwo__checkBrightness();

						if (brightnessRequest != BRIGHTNESS_NO_REQUEST)
						{
							QtwoState = QTWO_STATE_BRIGHTNESS_TRANSITION;
						}
						else
						{
							/* nothing to do */
						}
					}
				}
			}

			break;
		}

		case QTWO_STATE_TIME_TRANSITION:
		{
			if (Qtwo__updateTimeTransition(qtwoMode) == TIME_TRANSITION_FINISHED)
			{
				QtwoState = QTWO_STATE_IDLE;
			}

			break;
		}


		case QTWO_STATE_BRIGHTNESS_TRANSITION:
		{
			if (Qtwo__updateBrightness(qtwoMode) == BRIGHTNESS_NO_REQUEST)
			{
				QtwoState = QTWO_STATE_IDLE;
			}

			break;
		}

		case QTWO_STATE_SWITCH_BRIGHTNESS_LEVEL:
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

			QtwoState = QTWO_STATE_BRIGHTNESS_TRANSITION;

			break;
		}

		case QTWO_STATE_SWITCH_AUTO_COLOR:
		{
			modeAutoColor = TRUE;
			modeAutoColorPrev = FALSE;
			clockHoursColorPrev = Clock__getHours();
			targetColor = 0; /* set to white */
			QtwoState = QTWO_STATE_NEW_COLOR;

			break;
		}

		case QTWO_STATE_SWITCH_MANUAL_COLOR:
		{
			modeAutoColor = FALSE;
			QtwoState = QTWO_STATE_SET_NEXT_COLOR;

			break;
		}

		case QTWO_STATE_SET_NEXT_COLOR:
		{
			targetColor = currentColor;

			if (targetColor < (QTWO_COLOR_NB - 1))
			{
				targetColor++;
			}
			else
			{
				targetColor = 0;
			}

			QtwoState = QTWO_STATE_NEW_COLOR;

			break;
		}

		case QTWO_STATE_NEW_COLOR:
		{
			if (currentColor != targetColor)
			{
				currentColor = targetColor;
				Qtwo__eepromStorage();
			}

			if ((modeAutoColor == TRUE) && (modeAutoColorPrev == FALSE))
			{
				Qtwo__clearMatrix();
				modeAutoColorPrev = TRUE;
				QtwoState = QTWO_STATE_TIME_TRANSITION;
			}
			else
			{
				QtwoState = QTWO_STATE_IDLE;
			}

			break;
		}
	}

	if (qtwoMode != QTWO_MODE_SETUP)
	{
		Qtwo__updateLeds(); /* --> send LED colors to LED matrix */
	}
	else
	{
		if (QtwoSetupTimer - uC__getTaskIncrement()> 0)
		{
			QtwoSetupTimer = QtwoSetupTimer - uC__getTaskIncrement();

			if (QtwoSetupDisplayOn == TRUE)
			{
				Qtwo__updateLeds(); /* --> send LED colors to LED matrix */
			}
			else
			{
				LEDMatrix__clearMatrix(); /* --> send LED colors to LED matrix */
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

	qtwoReInit = FALSE;
}


static void Qtwo__reInitStateMachine (void)
{
	/* reInit time transition */
	clockHoursPrev = Clock__getHours();
	clockMinPrev = Clock__getMinutes();
	clockSecPrev = Clock__getSeconds();
	clockHoursColorPrev = clockHoursPrev;
	colorTransTimer = 0;
	timeTransition = TIME_TRANSITION_FINISHED;

	/* reInit brightness */
	LDRstate = LDR_STATE_1;
	currentBrightness = 0;
	brightnessRGBLevel = currentBrightnessTable[currentColor][currentBrightness];

	/* trigger new time */
	QtwoState = QTWO_STATE_IDLE;

	/* set reInit flag */
	qtwoReInit = TRUE;
}


void Qtwo__modeTransition (void)
{
	Qtwo__clearMatrix();
	Qtwo__reInitStateMachine();
}
