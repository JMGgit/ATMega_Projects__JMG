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

#if (CLOCK_TYPE != CLOCK_TYPE_OFF)

#define CLOCK_MODE_ONE_COLOR					1
#define CLOCK_MODE_COLOR_BLENDING				2
#define CLOCK_MODE_COLOR_BLENDING_INVERTED		3
#define CLOCK_MODE_COLOR_BLENDING_FAST			4
#define CLOCK_MODE_COLOR_BLENDING_INVERTED_FAST	5


void ModeClock__init (void);
void ModeClock__x10 (uint8_t clockMode);

#endif

#endif /* MODE_CLOCK_H_ */
