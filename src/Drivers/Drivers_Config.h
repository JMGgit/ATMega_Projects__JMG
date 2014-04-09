/*
 * Drivers_Config.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef DRIVERS_CONFIG_H_
#define DRIVERS_CONFIG_H_

#include "../Main/Main_Config.h"

/*********** CTL LED ***********/
#define UC_LED_DDR		DDRD
#define UC_LED_PORT		PORTD
#define UC_LED_PIN		PD5
#define CLOCK_LED_DDR	DDRD
#define CLOCK_LED_PORT	PORTD
#define CLOCK_LED_PIN	PD6
#define TEST_LED_DDR	DDRD
#define TEST_LED_PORT	PORTD
#define TEST_LED_PIN	PD7


/********* BUTTONS ********/
#define BUTTON_MODE_DDR		DDRC
#define BUTTON_MODE_PORT	PINC
#define BUTTON_MODE_PIN		PC2
#define BUTTON_FUNC1_DDR	DDRC
#define BUTTON_FUNC1_PORT	PINC
#define BUTTON_FUNC1_PIN	PC3
#define BUTTON_FUNC2_DDR	DDRC
#define BUTTON_FUNC2_PORT	PINC
#define BUTTON_FUNC2_PIN	PC4


/********** USART *********/
#define USART_REQESTER_MODE			0x40
#define USART_REQESTER_BUTTON		0x30
#define USART_REQESTER_COLOR		0x10
#define USART_REQESTER_CLOCK		0x80
#define USART_DATA_LENGTH_MODE		1
#define USART_DATA_LENGTH_BUTTON	2
#define USART_DATA_LENGTH_COLOR		4
#define USART_DATA_LENGTH_CLOCK		8
#define USART_DATA_LENGTH_MAX		4


#endif /* DRIVERS_CONFIG_H_ */
