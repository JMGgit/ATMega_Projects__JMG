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
	currentMode = mode;
}


void Modes__eepromStorage (void)
{

}


void Modes__init (void)
{
	Modes__setMode(MODE__STANDBY);
	Mode_Standby__init();
}


void Modes__x10 (void)
{
	switch (currentMode)
	{
		case MODE__STANDBY:
		{
			Mode_Standby__x10();

			if (Modes__standbyToSetup())
			{
				Modes__setMode(MODE__SETUP);
				Mode_Setup__init();
			}

			if (Modes__standbyToMeasurementStart())
			{
				Modes__setMode(MODE__MEASUREMENT_START);
			}

			break;
		}

		case MODE__SETUP:
		{
			Mode_Setup__x10();

			if (Modes__setupToStandby())
			{
				Modes__setMode(MODE__STANDBY);
				Mode_Standby__init();
			}

			break;
		}

		case MODE__MEASUREMENT_START:
		{
			Mode_MeasurementStart__x10();

			if (Modes__measurementStartToMeasurement())
			{
				Modes__setMode(MODE__MEASUREMENT);
			}

			if (Modes__measurementStartToStandby())
			{
				Modes__setMode(MODE__STANDBY);
			}

			break;
		}

		case MODE__MEASUREMENT:
		{
			Mode_Measurement__x10();

			if (Modes__measurementToMeasurementStats())
			{
				Modes__setMode(MODE__MEASUREMENT_STATS);
			}

			if (Modes__measurementToStandby())
			{
				Modes__setMode(MODE__STANDBY);
			}

			break;
		}

		case MODE__MEASUREMENT_STATS:
		{
			Mode_MeasurementStats__x10();

			if (Modes__measurementStatsToMeasurement())
			{
				Modes__setMode(MODE__MEASUREMENT);
			}

			break;
		}
	}

}
