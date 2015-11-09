/*
 * Modes.c
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */


#include "Modes.h"


static uint8_t currentMode;
static uint8_t previousMode;
uint8_t mode_EEPROM EEMEM;


void Modes__setMode (uint8_t mode)
{
	currentMode = mode;
}

static void Modes__eepromInit (void)
{
	currentMode = eeprom_read_byte(&mode_EEPROM);
	Mode_SetupMeasurement__eepromInit();
	Mode_Measurement__eepromInit();

	if (currentMode == 0xFF)
	{
		currentMode = MODE__STANDBY;
	}
}


void Modes__init (void)
{
	Modes__eepromInit();

	switch (currentMode)
	{
		case MODE__STANDBY:
		{
			Mode_Standby__init();
			break;
		}

		case MODE__SETUP:
		{
			Mode_Setup__init();
			break;
		}

		case MODE__SETUP_TIME:
		{
			Mode_SetupTime__init();
			break;
		}

		case MODE__SETUP_MEASUREMENT:
		{
			Mode_SetupMeasurement__init();
			break;
		}

		case MODE__MEASUREMENT_START:
		{
			Mode_MeasurementStart__init();
			break;
		}

		case MODE__MEASUREMENT:
		{
			Mode_Measurement__init();
			break;
		}

		case MODE__MEASUREMENT_STATS:
		{
			currentMode = MODE__MEASUREMENT;
			Mode_Measurement__init();
			break;
		}

		case MODE__STATS:
		{
			Mode_Stats__init();
			break;
		}

		case MODE__STATS_MESURE:
		{
			currentMode = MODE__STATS;
			Mode_Stats__init();
			break;
		}

		case MODE__TRANSFER:
		{
			currentMode = MODE__STANDBY;
			Mode_Standby__init();
			break;
		}
	}
}


static void Mode__eepromStorage (void)
{
	eeprom_update_byte(&mode_EEPROM, currentMode);
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

			if (Modes__standbyToStats())
			{
				Modes__setMode(MODE__STATS);
				Mode_Stats__init();
			}

			if (Modes__standbyToTransfer())
			{
				Modes__setMode(MODE__TRANSFER);
				Mode_Transfer__init();
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
				/* no reinit */
			}

			break;
		}

		case MODE__STATS:
		{
			uint8_t measure;

			Mode_Stats__x10();

			if (Modes__statsToStandby())
			{
				Modes__setMode(MODE__STANDBY);
				Mode_Standby__init();
			}

			if (Modes__statsToStatsMeasure(&measure))
			{
				Modes__setMode(MODE__STATS_MESURE);
				Mode_StatsMeasure__init(measure);
			}

			break;
		}

		case MODE__STATS_MESURE:
		{
			Mode_StatsMeasure__x10();

			if (Modes__statsMeasureToStats())
			{
				Modes__setMode(MODE__STATS);
				Mode_Stats__init();
			}

			break;
		}

		case MODE__TRANSFER:
		{
			Mode_Transfer__x10();

			if (Modes__transferToStandby())
			{
				Modes__setMode(MODE__STANDBY);
				Mode_Standby__init();
			}

			break;
		}


		default:
		{
			Modes__setMode(MODE__STANDBY);
			Mode_Standby__init();
		}
	}

	Mode__eepromStorage();
}
