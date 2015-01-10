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

void Snake__init (void);
void Snake__updateMatrix (void);


#define SNAKE_TIMER_INIT 		50

#endif /* SNAKE_H_ */
