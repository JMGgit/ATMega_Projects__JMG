/*
 * LedTable_Main_Config.h
 *
 *  Created on: 10.04.2016
 *      Author: Jean-Martin George
 */

#ifndef DRIVERS_MAIN_CONFIG_H_
#define DRIVERS_MAIN_CONFIG_H_


#include "Drivers_Config_Def.h"


#if (PROJECT == PROJECT__LED_TABLE)

/* WS2801 **/
#define LEDS_NB						(LED_MATRIX_SIZE_COL * LED_MATRIX_SIZE_LIN)
#define RGB_LED_ORDER				RGB_LED_ORDER__CONFIGURABLE

/* DCF77 */
#define DCF77_DATA_DDR				DDRB
#define DCF77_DATA_PORT				PINB
#define DCF77_DATA_PIN				PB0
#define DCF77_TASK_TIME				100

/* CTL LED */
#define UC_LED_DDR					DDRD
#define UC_LED_PORT					PORTD
#define UC_LED_PIN					PD4
#define CLOCK_LED_DDR				DDRD
#define CLOCK_LED_PORT				PORTD
#define CLOCK_LED_PIN				PD5
#define TEST1_LED_DDR				DDRD
#define TEST1_LED_PORT				PORTD
#define TEST1_LED_PIN				PD6
#define TEST2_LED_DDR				DDRD
#define TEST2_LED_PORT				PORTD
#define TEST2_LED_PIN				PD7
#define TEST3_LED_DDR				DDRA
#define TEST3_LED_PORT				PORTA
#define TEST3_LED_PIN				PA7
#define DCF77_LED_DDR				TEST1_LED_DDR
#define DCF77_LED_PORT				TEST1_LED_PORT
#define DCF77_LED_PIN				TEST1_LED_PIN
#define IRMP_LED_DDR				TEST2_LED_DDR
#define IRMP_LED_PORT				TEST2_LED_PORT
#define IRMP_LED_PIN				TEST2_LED_PIN
#define RUNTIME_LED_DDR 			TEST2_LED_DDR
#define RUNTIME_LED_PORT 			TEST2_LED_PORT
#define RUNTIME_LED_PIN				TEST2_LED_PIN
#define RUNTIME_OSC_DDR 			TEST3_LED_DDR
#define RUNTIME_OSC_PORT 			TEST3_LED_PORT
#define RUNTIME_OSC_PIN				TEST3_LED_PIN

/* USART */

#define USART_REQESTER_MODE			0x40
#define USART_REQESTER_BUTTON		0x30
#define USART_REQESTER_COLOR		0x10
#define USART_REQESTER_CLOCK		0x80

#define USART_DATA_LENGTH_MODE		1
#define USART_DATA_LENGTH_BUTTON	2
#define USART_DATA_LENGTH_COLOR		4
#define USART_DATA_LENGTH_CLOCK		8
#define USART_DATA_LENGTH_READ_MAX	4

#endif

#endif /* LEDTABLE_DRIVERS_CONFIG_H_ */
