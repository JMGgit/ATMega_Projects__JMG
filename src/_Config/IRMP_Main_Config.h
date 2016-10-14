/*
 * IRMP_Main_Config.h
 *
 *  Created on: 10.04.2016
 *      Author: Jean-Martin George
 */

#ifndef IRMP_MAIN_CONFIG_H_
#define IRMP_MAIN_CONFIG_H_


#include "Main_Config_Def.h"


#if (PROJECT == PROJECT__IRMP)

#define CLOCK_TYPE						CLOCK_TYPE_OFF
#define CLOCK_SYNC						CLOCK_SYNC_OFF
#define BUTTONS_WIRED					BUTTONS_WIRED_OFF
#define BUTTONS_USART					BUTTONS_USART_OFF
#define BUTTONS_IRMP					BUTTONS_IRMP_SEND_TO_TWI
#define BUTTONS_TWI						BUTTONS_TWI_OFF
#define LCD_CONTROLLER					LCD_CONTROLLER_OFF
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_OFF
#define RUNTIME_TEST					RUNTIME_TEST_OFF
#define DEBUG_MODE						DEBUG_MODE_OFF

#endif


#endif /* IRMP_MAIN_CONFIG_H_ */
