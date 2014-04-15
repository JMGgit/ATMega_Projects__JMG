/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes.h"


static Mode_t currentMode;

void Modes__setMode (Mode_t mode)
{
	if (mode < MODE_NB)
	{
		currentMode = mode;
	}
	else
	{
		currentMode = MODE__QLOCKTWO;
	}
}


static void Modes__setNextMode (void)
{
	Modes__setMode(currentMode + 1);
}


void Modes__eepromStorage (void)
{
}



void Modes__init (void)
{
	Modes__setMode(MODE__QLOCKTWO);
}


void Modes__x10 (void)
{
	if (Buttons__isPressedOnce(&buttonMode))
	{
		Modes__setNextMode();
	}
}
