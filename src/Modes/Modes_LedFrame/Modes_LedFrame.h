/*
 * Modes.h
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MODES_LEDFRAME_H_
#define MODES_LEDFRAME_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Buttons.h"
#include "Mode_Startup.h"
#include "Mode_AllOn.h"
#include "Mode_ColorBlending.h"
#include "Mode_DoubleColor.h"


typedef enum
{
	MODE__STARTUP = 0,
	MODE__OFF,
	MODE__BLENDING_SLOW_2_COLORS,
	MODE__BLENDING_SWEEP_FAST,
	MODE__DOUBLE_COLOR,
	MODE_NB
} Mode_t;

#define MODE__INIT (MODE__OFF + 1)


#endif /* MODES_LEDFRAME_H_ */
