/*
 * Modes.h
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MODES_H_
#define MODES_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "Qlocktwo/Mode_Qlocktwo.h"
#include "Startup/Mode_Startup.h"


typedef enum
{
	MODE__STARTUP = 0,
	MODE__TIME_SETUP,
	MODE__QLOCKTWO,
	MODE__SECONDS,
	MODE__OFF,
	MODE_NB
} Mode_t;


/* public functions */
void Modes__init (void);
void Modes__x10 (void);
void Modes__setMode (uint8_t mode);
void Modes__eepromStorage (void);


#endif /* MODES_H_ */
