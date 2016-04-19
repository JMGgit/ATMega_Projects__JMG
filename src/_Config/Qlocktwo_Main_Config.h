/*
 * Qlocktwo_Main_Config.h
 *
 *  Created on: 10.04.2016
 *      Author: Jean-Martin George
 */

#ifndef QLOCKTWO_MAIN_CONFIG_H_
#define QLOCKTWO_MAIN_CONFIG_H_


#include "Main_Config_Def.h"


#if (PROJECT == PROJECT__QLOCKTWO)

#define CLOCK_TYPE						CLOCK_TYPE_DS1307
#define CLOCK_SYNC						CLOCK_SYNC_DCF77
#define BUTTONS_USART					BUTTONS_USART_OFF
#if (QLOCKTWO_REVISION == QLOCKTWO_REVISION_2)
#define BUTTONS_WIRED					BUTTONS_WIRED_HC165
#define BUTTONS_IRMP					BUTTONS_IRMP_OFF
#define BUTTON_OFF_AVAILABLE			BUTTON_OFF_AVAILABLE_NO
#define BUTTON_FUNC3_AVAILABLE			BUTTON_FUNC3_AVAILABLE_NO
#endif

#if (	(QLOCKTWO_REVISION == QLOCKTWO_REVISION_3)\
	||	(QLOCKTWO_REVISION == QLOCKTWO_REVISION_4)\
	)
#define BUTTONS_WIRED					BUTTONS_WIRED_OFF
#define BUTTONS_IRMP					BUTTONS_IRMP_USED
#define BUTTON_OFF_AVAILABLE			BUTTON_OFF_AVAILABLE_YES
#define BUTTON_FUNC3_AVAILABLE			BUTTON_FUNC3_AVAILABLE_YES
#endif

#define BUTTONS_TWI						BUTTONS_TWI_OFF
#define LCD_CONTROLLER					LCD_CONTROLLER_OFF
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_OFF
#define RUNTIME_TEST					RUNTIME_TEST_OFF
#define QLOCKTWO_LANG					QLOCKTWO_LANG_MULTI

#if (QLOCKTWO_REVISION == QLOCKTWO_REVISION_2)
#define MODE_SNAKE						MODE_SNAKE_OFF
#endif

#if (	(QLOCKTWO_REVISION == QLOCKTWO_REVISION_3)\
	||	(QLOCKTWO_REVISION == QLOCKTWO_REVISION_4)\
	)
#define MODE_SNAKE						MODE_SNAKE_ON
#define SNAKE_SPEED						50
#endif

#define LED_MATRIX_SIZE_LIN				10
#define LED_MATRIX_SIZE_COL				11

#define WHITE_COLOR_PERCENT_RED			70
#define WHITE_COLOR_PERCENT_GREEN		100
#define WHITE_COLOR_PERCENT_BLUE		100

#define QTWO_BRIGHTNESS_LEVEL_1			4
#define QTWO_BRIGHTNESS_LEVEL_2			10
#define QTWO_BRIGHTNESS_LEVEL_3			25
#define QTWO_BRIGHTNESS_LEVEL_4			90
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_1	10
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_2	25
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_3	90
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_4	160
#define SNAKE_BRIGHTNESS_LEVEL			30

#endif

#endif /* QLOCKTWO_MAIN_CONFIG_H_ */
