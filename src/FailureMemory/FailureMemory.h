/*
 * FailureMemory.h
 *
 *  Created on: 09.10.2016
 *      Author: Jean-Martin George
 */

#ifndef FAILUREMEMORY_H_
#define FAILUREMEMORY_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Debug.h"

#if (DEBUG_MODE == DEBUG_MODE_ON)

#define FAILURE_MEMORY_SIZE 15


typedef struct
{
	uint8_t begin;

	uint8_t errorID;
	uint8_t driversState;
	uint8_t mainState;
	uint8_t modeState;
	uint8_t whileState;
#if (CLOCK_TYPE != CLOCK_TYPE_OFF)
	debugTime_t time;
#endif
#if (PROJECT != PROJECT__IRMP)
	uint8_t mode;
#endif
#if (PROJECT == PROJECT__QLOCKTWO)
	RGB_Color_t qtwoCcolor;
	uint8_t qtwoBrightness;
#endif

	uint8_t end;
} FailureMemory_t;


void FailureMemory__init (void);
void FailureMemory__enterFault (uint8_t errorID);
void FailureMemory__clearAll (void);
void FailureMemory__eepromStorage (void);
void FailureMemory__disableStorage (void);
uint8_t FailureMemory__getFaultCounter (void);
uint8_t FailureMemory__getErrortID (uint8_t entryIdx);

#define FAILURE_ID__TEST		0x01
#define FAILURE_ID__WATCHDOG	0x02
#define FAILURE_ID__REPORT		0x03

#endif /* (DEBUG_MODE == DEBUG_MODE_ON) */

#endif /* FAILUREMEMORY_H_ */
