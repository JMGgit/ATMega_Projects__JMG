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
	Lcd__init();

	setOutput(TEST_LED_DDR, TEST_LED_PIN);

	setInput(BUTTON_MODE_DDR, BUTTON_MODE_PIN);
	setHigh(BUTTON_MODE_PORT, BUTTON_MODE_PIN);
	setInput(BUTTON_FUNC1_DDR, BUTTON_FUNC1_PIN);
	setHigh(BUTTON_FUNC1_PORT, BUTTON_FUNC1_PIN);
	setInput(BUTTON_FUNC2_DDR, BUTTON_FUNC2_PIN);
	setHigh(BUTTON_FUNC2_PORT, BUTTON_FUNC2_PIN);

	while (1)
	{
		if (uC__isTaskTrigger_x10())
		{
			Drivers__x10();
			Lcd__x10();
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
