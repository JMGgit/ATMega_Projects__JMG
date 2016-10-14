/*
 * FailureMemory.c
 *
 *  Created on: 09.10.2016
 *      Author: Jean-Martin George
 */


#include "FailureMemory.h"
#include "Drivers.h"

#if (DEBUG_MODE == DEBUG_MODE_ON)

static FailureMemory_t failureMemory[FAILURE_MEMORY_SIZE];
static FailureMemory_t failureMemory_EEPROM[FAILURE_MEMORY_SIZE] EEMEM;

static uint8_t failureMemoryCounter;
static uint8_t failureMemoryCounter_EEPROM EEMEM;

static uint8_t storageDisabled;


void FailureMemory__init (void)
{
	if (eeprom_read_byte(&failureMemoryCounter_EEPROM) <= FAILURE_MEMORY_SIZE)
	{
		failureMemoryCounter = eeprom_read_byte(&failureMemoryCounter_EEPROM);
		eeprom_read_block(failureMemory, failureMemory_EEPROM, sizeof(FailureMemory_t)* FAILURE_MEMORY_SIZE);

	}
	else
	{
		failureMemoryCounter = 0;
		FailureMemory__clearAll();
	}
}


void FailureMemory__enterFault (uint8_t errorID)
{
	if ((failureMemoryCounter < FAILURE_MEMORY_SIZE) && (storageDisabled == FALSE))
	{
		failureMemory[failureMemoryCounter].begin = 0xAB;
		failureMemory[failureMemoryCounter].end = 0xCD;
		failureMemory[failureMemoryCounter].errorID = errorID;
#if (CLOCK_TYPE != CLOCK_TYPE_OFF)
		Debug__getTime(&failureMemory[failureMemoryCounter].time);
#endif
#if (PROJECT != PROJECT__IRMP)
		Debug__getMode(&failureMemory[failureMemoryCounter].mode);
#endif
		Debug__getMainState(&failureMemory[failureMemoryCounter].mainState);
		Debug__getDriversState(&failureMemory[failureMemoryCounter].driversState);
		Debug__getModeState(&failureMemory[failureMemoryCounter].modeState);
		Debug__getWhileState(&failureMemory[failureMemoryCounter].whileState);
#if (PROJECT == PROJECT__QLOCKTWO)
		Debug__getQtwoColor(&failureMemory[failureMemoryCounter].qtwoCcolor);
		Debug__getQtwoBrightness(&failureMemory[failureMemoryCounter].qtwoBrightness);
#endif
		failureMemoryCounter++;

		FailureMemory__eepromStorage();
	}
}


void FailureMemory__clearAll (void)
{
	memset(&failureMemory[0], 0, sizeof(FailureMemory_t) * FAILURE_MEMORY_SIZE);
	failureMemoryCounter = 0;

	FailureMemory__eepromStorage();
}


void FailureMemory__eepromStorage (void)
{
	uC__disableWatchdog();
	eeprom_update_block(&failureMemory[0], &failureMemory_EEPROM[0], sizeof(FailureMemory_t) * FAILURE_MEMORY_SIZE);
	eeprom_update_byte(&failureMemoryCounter_EEPROM, failureMemoryCounter);
	uC__enableWatchdog();
}


uint8_t FailureMemory__getFaultCounter (void)
{
	return (failureMemoryCounter);
}


uint8_t FailureMemory__getErrortID (uint8_t entryIdx)
{
	return (failureMemory[entryIdx].errorID);
}


void FailureMemory__disableStorage (void)
{
	storageDisabled = TRUE;
}

#endif /* (DEBUG_MODE == DEBUG_MODE_ON) */
