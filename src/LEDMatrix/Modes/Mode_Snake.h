/*
 * Snake.h
 *
 *  Created on: 01.08.2013
 *      Author: Jean-Martin George
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "LEDMatrix.h"
#include "Clock.h"
#include "Buttons.h"

#if (MODE_SNAKE == MODE_SNAKE_ON)

void Snake__init (void);
void Snake__x10 (uint8_t brightness);

#define SNAKE_TIMER_INIT 		SNAKE_SPEED

#endif

#endif /* SNAKE_H_ */
