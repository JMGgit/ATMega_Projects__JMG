/*
 * Main.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Main.h"

#define EEPROM_STORAGE_TIME	360000 /* 1 hour */

static uint32_t eepromCounter = EEPROM_STORAGE_TIME;


int main (void)
{
	Drivers__init();
	Clock__init();
	Modes__init();

	while (1)
	{
		if (uC__isTaskTrigger_x10())
		{
			Drivers__x10();
			Clock__x10();
			Buttons__x10();
			Modes__x10();

			if (eepromCounter > 0)
			{
				eepromCounter--;
			}
			else
			{
				Modes__eepromStorage();
				eepromCounter = EEPROM_STORAGE_TIME;
			}
		}
	}

	return 0;
}
