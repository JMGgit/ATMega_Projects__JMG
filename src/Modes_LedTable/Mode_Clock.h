/*
 * Mode_Clock.h
 *
 *  Created on: 07.06.2013
 *      Author: Jean-Martin George
 */

#ifndef MODE_CLOCK_H_
#define MODE_CLOCK_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "LEDMatrix.h"
#include "Clock.h"

#define CLOCK_MODE_ONE_COLOR					1
#define CLOCK_MODE_COLOR_BLENDING				2
#define CLOCK_MODE_COLOR_BLENDING_INVERTED		3

void Clock__updateMatrix (uint8_t clockMode);

#endif /* MODE_CLOCK_H_ */
