/*
 * LedTable_Main_Config.h
 *
 *  Created on: 10.04.2016
 *      Author: Jean-Martin George
 */

#ifndef LEDTABLE_MAIN_CONFIG_H_
#define LEDTABLE_MAIN_CONFIG_H_


#include "Main_Config_Def.h"


#if (PROJECT == PROJECT__LED_TABLE)

#define CLOCK_TYPE						CLOCK_TYPE_DS1307
#define CLOCK_SYNC						CLOCK_SYNC_DCF77
#define BUTTONS_WIRED					BUTTONS_WIRED_OFF
#define BUTTONS_USART					BUTTONS_USART_ON
#define BUTTONS_IRMP					BUTTONS_IRMP_USED
#define BUTTONS_TWI						BUTTONS_TWI_OFF
#define LCD_CONTROLLER					LCD_CONTROLLER_OFF
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_OFF
#define RUNTIME_TEST					RUNTIME_TEST_OFF
#define MODE_SNAKE						MODE_SNAKE_ON
#define SNAKE_SPEED						50

#define LED_MATRIX_SIZE_LIN				13
#define LED_MATRIX_SIZE_COL				23

#define WHITE_COLOR_PERCENT_RED			70
#define WHITE_COLOR_PERCENT_GREEN		100
#define WHITE_COLOR_PERCENT_BLUE		100

#define SNAKE_BRIGHTNESS_LEVEL			255

#endif

#endif /* LEDTABLE_MAIN_CONFIG_H_ */
