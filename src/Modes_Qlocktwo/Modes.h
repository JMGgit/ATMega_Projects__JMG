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
#if (OFF_BUTTON != OFF_BUTTON_MODE)
	MODE__OFF,
#endif
	MODE__TIME_SETUP,
	MODE__QLOCKTWO,
	MODE__SECONDS,
#if (PROJECT == PROJECT__QLOCKTWO_3_0)
	MODE__SNAKE,
#endif
#if (OFF_BUTTON == OFF_BUTTON_MODE)
	MODE__OFF,
#endif
	MODE_NB
} Mode_t;


/* public functions */
void Modes__init (void);
void Modes__x10 (void);
void Modes__setMode (Mode_t mode);
void Modes__eepromStorage (void);


#endif /* MODES_H_ */
