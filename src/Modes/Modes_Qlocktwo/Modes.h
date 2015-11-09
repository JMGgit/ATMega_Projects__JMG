/*
 * Modes.h
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MODES_H_
#define MODES_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Mode_Qlocktwo.h"
#include "Mode_Startup.h"
#include "Mode_Snake.h"


typedef enum
{
	MODE__STARTUP = 0,
#if (BUTTON_OFF_AVAILABLE != BUTTON_OFF_AVAILABLE_NO)
	MODE__OFF,
#endif
	MODE__TIME_SETUP,
	MODE__QLOCKTWO,
	MODE__SECONDS,
#if (MODE_SNAKE == MODE_SNAKE_ON)
	MODE__SNAKE,
#endif
#if (BUTTON_OFF_AVAILABLE == BUTTON_OFF_AVAILABLE_NO)
	MODE__OFF,
#endif
	MODE_NB
} Mode_t;


/* public functions */
void Modes__init (void);
void Modes__x10 (void);
void Modes__setMode (Mode_t mode);
Mode_t Modes__getMode (void);
void Modes__Start (void);


#endif /* MODES_H_ */
