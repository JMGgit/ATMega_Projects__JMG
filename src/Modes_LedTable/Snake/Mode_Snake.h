/*
 * Snake.h
 *
 *  Created on: 01.08.2013
 *      Author: Jean-Martin George
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#include "../../Main/Main_Types.h"
#include "../../Main/Main_Config.h"
#include "../../LEDMatrix/LEDMatrix.h"
#include "../../Clock/Clock.h"
#include "../../Buttons/Buttons.h"

void Snake__init (void);
void Snake__updateMatrix (void);

/* 800ms */
#define SNAKE_TIMER_INIT 		100
#define SNAKE_SPEED_INCREMENT	30

#endif /* SNAKE_H_ */
