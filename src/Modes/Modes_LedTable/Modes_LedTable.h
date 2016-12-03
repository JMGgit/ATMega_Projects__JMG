/*
 * Modes.h
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MODES_LEDTABLE_H_
#define MODES_LEDTABLE_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Mode_Startup.h"
#include "Mode_AllOn.h"
#include "Mode_ColorBlending.h"
#include "Mode_Clock.h"
#include "Mode_Snake.h"
#include "Mode_Off.h"
#include "Mode_LEDScreen.h"


typedef enum
{
	MODE__STARTUP = 0,
	MODE__FAILUREMEMORY,
	MODE__OFF,
	MODE__ALL_ON,
	MODE__BLENDING_SLOW,
	MODE__BLENDING_SLOW_2_COLORS,
	MODE__BLENDING_FAST,
	MODE__BLENDING_FAST_2_COLORS,
	MODE__BLENDING_SWEEP,
	MODE__BLENDING_SWEEP_FAST,
	MODE__BLENDING_CLOCK,
	MODE__BLENDING_CLOCK_INVERTED,
	MODE__BLENDING_CLOCK_FAST,
	MODE__BLENDING_CLOCK_INVERTED_FAST,
	MODE__CLOCK,
	MODE__SNAKE,
	MODE_NB
} Mode_t;


#define MODE__INIT (MODE__OFF + 1)


void Modes__toggleStartupMode (void);

#endif /* MODES_LEDTABLE_H_ */
