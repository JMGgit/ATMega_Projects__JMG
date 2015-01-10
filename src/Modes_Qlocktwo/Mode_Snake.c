/*
 * Snake.c
 *
 *  Created on: 01.08.2013
 *      Author: Jean-Martin George
 */

#include "Mode_Snake.h"


#define ST_EMPTY				0x00
#define ST_FIRST				0x01
#define ST_DIRECTION_UP			0x02
#define ST_DIRECTION_DOWN		0x03
#define ST_DIRECTION_LEFT		0x04
#define ST_DIRECTION_RIGHT		0x05

#define SNAKE_COLOR_R			0x00
#define SNAKE_COLOR_G			0xFF
#define SNAKE_COLOR_B			0x00

#define APPLE_COLOR_R			0xFF
#define APPLE_COLOR_G			0x00
#define APPLE_COLOR_B			0x00

typedef struct
{
	uint8_t first_posLine;
	uint8_t first_posCol;
	uint8_t last_posLine;
	uint8_t last_posCol;
	uint8_t speed;
} SnakeState_t;

typedef struct
{
	uint8_t pos_line;
	uint8_t pos_col;
	uint8_t caught;
} AppleState_t;


static SnakeState_t snake;
static uint8_t direction;
static uint8_t newDirection = FALSE;
static AppleState_t apple;
static uint8_t timer;
static uint8_t SnakeMatrix[LED_MATRIX_SIZE_LIN + 1][LED_MATRIX_SIZE_COL + 1];


void Snake__init (void)
{
	uint16_t linIt, colIt;

	timer = SNAKE_TIMER_INIT;

	for (linIt = 1; linIt <= LED_MATRIX_SIZE_LIN; linIt++)
	{
		for (colIt = 1; colIt <= LED_MATRIX_SIZE_COL; colIt++)
		{
			SnakeMatrix[linIt][colIt] = ST_EMPTY;
		}
	}

	snake.first_posLine = 1;
	snake.first_posCol = 4;
	snake.last_posLine = 1;
	snake.last_posCol = 1;
	apple.caught = FALSE;
	snake.speed = 1;
	apple.pos_line = snake.first_posLine;
	apple.pos_col = snake.first_posCol;

	direction = ST_DIRECTION_RIGHT;

	SnakeMatrix[1][1] = ST_DIRECTION_RIGHT;
	SnakeMatrix[1][2] = ST_DIRECTION_RIGHT;
	SnakeMatrix[1][3] = ST_DIRECTION_RIGHT;
	SnakeMatrix[1][4] = ST_FIRST;

	/* random number generator init */
	srand(Clock__getSeconds());

	/* apple position */
	while (SnakeMatrix[apple.pos_line][apple.pos_col] != ST_EMPTY)
	{
		apple.pos_line = (uint8_t)(rand() % LED_MATRIX_SIZE_LIN + 1);
		apple.pos_col = (uint8_t)(rand() % LED_MATRIX_SIZE_COL + 1);
	}
}

void Snake__updateMatrix (void)
{
	uint16_t linIt, colIt;

	/* calculate new direction */
	if (newDirection == FALSE)
	{
		/* change of direction only allowed one time until new position is calculated */
		if ((Buttons__isPressedOnce(&buttonLeft)) && (direction != ST_DIRECTION_RIGHT))
		{
			direction = ST_DIRECTION_LEFT;
			newDirection = TRUE;
		}
		else if ((Buttons__isPressedOnce(&buttonRight)) && (direction != ST_DIRECTION_LEFT))
		{
			direction = ST_DIRECTION_RIGHT;
			newDirection = TRUE;
		}
		else if ((Buttons__isPressedOnce(&buttonUp)) && (direction != ST_DIRECTION_DOWN))
		{
			direction = ST_DIRECTION_UP;
			newDirection = TRUE;
		}
		else if ((Buttons__isPressedOnce(&buttonDown)) && (direction != ST_DIRECTION_UP))
		{
			direction = ST_DIRECTION_DOWN;
			newDirection = TRUE;
		}
	}

	if (timer > 0)
	{
		timer--;
	}
	else
	{
		newDirection = FALSE;

		if (direction == ST_DIRECTION_LEFT)
		{
			SnakeMatrix[snake.first_posLine][snake.first_posCol] = ST_DIRECTION_LEFT;
			snake.first_posCol--;

			if (snake.first_posCol < 1)
			{
				snake.first_posCol = LED_MATRIX_SIZE_COL;
			}
		}
		else if (direction == ST_DIRECTION_RIGHT)
		{
			SnakeMatrix[snake.first_posLine][snake.first_posCol] = ST_DIRECTION_RIGHT;
			snake.first_posCol++;

			if (snake.first_posCol > LED_MATRIX_SIZE_COL)
			{
				snake.first_posCol = 1;
			}
		}
		else if (direction == ST_DIRECTION_UP)
		{
			SnakeMatrix[snake.first_posLine][snake.first_posCol] = ST_DIRECTION_UP;
			snake.first_posLine--;

			if (snake.first_posLine < 1)
			{
				snake.first_posLine = LED_MATRIX_SIZE_LIN;
			}
		}
		else if (direction == ST_DIRECTION_DOWN)
		{
			SnakeMatrix[snake.first_posLine][snake.first_posCol] = ST_DIRECTION_DOWN;
			snake.first_posLine++;

			if (snake.first_posLine > LED_MATRIX_SIZE_LIN)
			{
				snake.first_posLine = 1;
			}
		}

		if (apple.caught == FALSE)
		{
			if (SnakeMatrix[snake.last_posLine][snake.last_posCol] == ST_DIRECTION_LEFT)
			{
				SnakeMatrix[snake.last_posLine][snake.last_posCol] = ST_EMPTY;
				snake.last_posCol--;

				if (snake.last_posCol < 1)
				{
					snake.last_posCol = LED_MATRIX_SIZE_COL;
				}
			}
			else if (SnakeMatrix[snake.last_posLine][snake.last_posCol] == ST_DIRECTION_RIGHT)
			{
				SnakeMatrix[snake.last_posLine][snake.last_posCol] = ST_EMPTY;
				snake.last_posCol++;

				if (snake.last_posCol > LED_MATRIX_SIZE_COL)
				{
					snake.last_posCol = 1;
				}
			}
			else if (SnakeMatrix[snake.last_posLine][snake.last_posCol] == ST_DIRECTION_UP)
			{
				SnakeMatrix[snake.last_posLine][snake.last_posCol] = ST_EMPTY;
				snake.last_posLine--;

				if (snake.last_posLine < 1)
				{
					snake.last_posLine = LED_MATRIX_SIZE_LIN;
				}
			}
			else if (SnakeMatrix[snake.last_posLine][snake.last_posCol] == ST_DIRECTION_DOWN)
			{
				SnakeMatrix[snake.last_posLine][snake.last_posCol] = ST_EMPTY;
				snake.last_posLine++;

				if (snake.last_posLine > LED_MATRIX_SIZE_LIN)
				{
					snake.last_posLine = 1;
				}
			}
		}
		else
		{
			/* get new apple position */
			while (SnakeMatrix[apple.pos_line][apple.pos_col] != ST_EMPTY)
			{
				apple.pos_line = (uint8_t)(rand() % LED_MATRIX_SIZE_LIN + 1);
				apple.pos_col = (uint8_t)(rand() % LED_MATRIX_SIZE_COL + 1);
			}

			apple.caught = FALSE;
		}

		if ((snake.first_posCol == apple.pos_col) && (snake.first_posLine == apple.pos_line))
		{
			apple.caught = TRUE;
			snake.speed++;
		}
		else if (SnakeMatrix[snake.first_posLine][snake.first_posCol] != ST_EMPTY)
		{
			/* game over! */
			Snake__init();
		}

		SnakeMatrix[snake.first_posLine][snake.first_posCol] = ST_FIRST;
		timer = SNAKE_TIMER_INIT / snake.speed;

	}

	LEDMatrix__clearMatrix();

	LEDMatrix__setRGBColor(apple.pos_line, apple.pos_col, getRGBColorFromComponents(APPLE_COLOR_R, APPLE_COLOR_G, APPLE_COLOR_B));

	for (linIt = 1; linIt <= LED_MATRIX_SIZE_LIN; linIt++)
	{
		for (colIt = 1; colIt <= LED_MATRIX_SIZE_COL; colIt++)
		{
			if (SnakeMatrix[linIt][colIt] != ST_EMPTY)
			{
				LEDMatrix__setRGBColor(linIt, colIt, getRGBColorFromComponents(SNAKE_COLOR_R, SNAKE_COLOR_G, SNAKE_COLOR_B));
			}
		}
	}
}
