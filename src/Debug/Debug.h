/*
 * Debug.h
 *
 *  Created on: 09.10.2016
 *      Author: Jean-Martin George
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "Main_Types.h"
#include "Main_Config.h"
#if (PROJECT != PROJECT__IRMP)
#include "Modes.h"
#include "Clock.h"
#endif


#if (DEBUG_MODE == DEBUG_MODE_ON)

typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
} debugTime_t;


void Debug__captureData (void);
void Debug__getTime (debugTime_t *debugTime);
#if (PROJECT != PROJECT__IRMP)
void Debug__getMode (Mode_t *mode);
#endif
void Debug__getMainState (uint8_t *state);
void Debug__getDriversState (uint8_t *state);
void Debug__getModeState (uint8_t *state);
void Debug__getWhileState (uint8_t *state);


#if (PROJECT == PROJECT__QLOCKTWO)
void Debug__getQtwoColor (RGB_Color_t *color);
void Debug__getQtwoBrightness (uint8_t *brightness);
#endif

#endif /* (DEBUG_MODE == DEBUG_MODE_ON) */

#endif /* DEBUG_H_ */
