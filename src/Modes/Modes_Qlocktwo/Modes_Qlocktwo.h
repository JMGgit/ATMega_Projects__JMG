/*
 * Modes.h
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MODES_QLOCKTWO_H_
#define MODES_QLOCKTWO_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Mode_Qtwo.h"
#include "Mode_Startup.h"
#include "Mode_Snake.h"
#include "Mode_Off.h"


typedef enum
{
	MODE__STARTUP = 0,
	MODE__OFF,
	MODE__TIME_SETUP,
	MODE__QLOCKTWO,
	MODE__SECONDS,
	MODE__SNAKE,
	MODE_NB
} Mode_t;


void Modes__toggleStartupMode (void);


#endif /* MODES_QLOCKTWO_H_ */
