/*
 * TemperatureLogger_Main_Config.h
 *
 *  Created on: 10.04.2016
 *      Author: Jean-Martin George
 */

#ifndef TEMPERATURELOGGER_MAIN_CONFIG_H_
#define TEMPERATURELOGGER_MAIN_CONFIG_H_

#include "Main_Config_Def.h"

#if (PROJECT == PROJECT__TEMPERATURE_LOGGER)

#define CLOCK_TYPE						CLOCK_TYPE_DS1307
#define CLOCK_SYNC						CLOCK_SYNC_OFF
#define BUTTONS_WIRED					BUTTONS_WIRED_PIN
#define BUTTONS_USART					BUTTONS_USART_OFF
#define BUTTONS_IRMP					BUTTONS_IRMP_OFF
#define BUTTONS_TWI						BUTTONS_TWI_OFF
#define LCD_CONTROLLER					LCD_CONTROLLER_SPLC780D1
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_TEST
#define RUNTIME_TEST					RUNTIME_TEST_OFF
#define DEBUG_MODE						DEBUG_MODE_OFF

#define MAX_MEASUREMENT_POINTS			(1900 /2)
#define LCD_MATRIX_SIZE_LIN				4
#define LCD_MATRIX_SIZE_COL				20

#endif

#endif /* TEMPERATURELOGGER_MAIN_CONFIG_H_ */
