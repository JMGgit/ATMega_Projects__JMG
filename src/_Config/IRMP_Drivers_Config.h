/*
 * IRMP_Drivers_Config.h
 *
 *  Created on: 10.04.2016
 *      Author: Jean-Martin George
 */

#ifndef IRMP_DRIVERS_CONFIG_H_
#define IRMP_DRIVERS_CONFIG_H_


#include "Drivers_Config_Def.h"


#if (PROJECT == PROJECT__IRMP)

/* CTL LED */
#define UC_LED_DDR					DDRD
#define UC_LED_PORT					PORTD
#define UC_LED_PIN					PD4
#define TEST1_LED_DDR				DDRD
#define TEST1_LED_PORT				PORTD
#define TEST1_LED_PIN				PD6
#define TEST2_LED_DDR				DDRD
#define TEST2_LED_PORT				PORTD
#define TEST2_LED_PIN				PD7
#define TEST3_LED_DDR				DDRD
#define TEST3_LED_PORT				PORTD
#define TEST3_LED_PIN				PD3
#define IRMP_LED_DDR				TEST1_LED_DDR
#define IRMP_LED_PORT				TEST1_LED_PORT
#define IRMP_LED_PIN				TEST1_LED_PIN
#define RUNTIME_LED_DDR 			TEST2_LED_DDR
#define RUNTIME_LED_PORT 			TEST2_LED_PORT
#define RUNTIME_LED_PIN				TEST2_LED_PIN
#define RUNTIME_OSC_DDR 			TEST3_LED_DDR
#define RUNTIME_OSC_PORT 			TEST3_LED_PORT
#define RUNTIME_OSC_PIN				TEST3_LED_PIN

/* USART */
#define USART_DATA_LENGTH_READ_MAX	0

/* SPI not needed but speed has to be defined */
#define SPI_SPEED 					SPI_DISABLED

#endif

#endif /* IRMP_DRIVERS_CONFIG_H_ */
