/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes.h"


static Mode_t currentMode, previousMode;


void Modes__setMode (Mode_t mode)
{
	currentMode = mode;
}


void Modes__init (void)
{
	Modes__setMode(0xFF);
	Mode_SetupMeasurement__eepromInit();
	Mode_Measurement__eepromInit();
}


void Modes__x10 (void)
{
	if (previousMode != currentMode)
	{
		Lcd__requestRefresh();
		previousMode = currentMode;
	}

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
				Mode_MeasurementStart__init();
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

			if (Modes__setupToSetupTime())
			{
				Modes__setMode(MODE__SETUP_TIME);
				Mode_SetupTime__init();
			}

			if (Modes__setupToSetupMeasurement())
			{
				Modes__setMode(MODE__SETUP_MEASUREMENT);
				Mode_SetupMeasurement__init();
			}

			break;
		}

		case MODE__SETUP_TIME:
		{
			Mode_SetupTime__x10();

			if (Modes__setupTimeToSetup())
			{
				Modes__setMode(MODE__SETUP);
				Mode_Setup__init();
			}

			break;
		}

		case MODE__SETUP_MEASUREMENT:
		{
			Mode_SetupMeasurement__x10();

			if (Modes__setupMeasurementToSetup())
			{
				Modes__setMode(MODE__SETUP);
				Mode_Setup__init();
			}

			break;
		}

		case MODE__MEASUREMENT_START:
		{
			Mode_MeasurementStart__x10();

			if (Modes__measurementStartToMeasurement())
			{
				Modes__setMode(MODE__MEASUREMENT);
				Mode_Measurement__init();
			}

			if (Modes__measurementStartToStandby())
			{
				Modes__setMode(MODE__STANDBY);
				Mode_Standby__init();
			}

			break;
		}

		case MODE__MEASUREMENT:
		{
			Mode_Measurement__x10();

			if (Modes__measurementToMeasurementStats())
			{
				Modes__setMode(MODE__MEASUREMENT_STATS);
				Mode_MeasurementStats__init();
			}

			if (Modes__measurementToStandby())
			{
				Modes__setMode(MODE__STANDBY);
				Mode_Standby__init();
			}

			break;
		}

		case MODE__MEASUREMENT_STATS:
		{
			Mode_MeasurementStats__x10();

			if (Modes__measurementStatsToMeasurement())
			{
				Modes__setMode(MODE__MEASUREMENT);
				Mode_Measurement__init();
			}

			break;
		}

		default:
		{
			Modes__setMode(MODE__STANDBY);
			Mode_Standby__init();
		}
	}

}
