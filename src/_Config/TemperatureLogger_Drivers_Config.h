/*
 * TemperatureLogger_Drivers_Config.h
 *
 *  Created on: 10.04.2016
 *      Author: Jean-Martin George
 */

#ifndef TEMPERATURELOGGER_DRIVERS_CONFIG_H_
#define TEMPERATURELOGGER_DRIVERS_CONFIG_H_


#include "Drivers_Config_Def.h"


#if (PROJECT == PROJECT__TEMPERATURE_LOGGER)

/* DS1307 */
#define DS1307_MODE					DS1307_MODE_TWI

/*********** CTL LED ***********/
#define UC_LED_DDR		DDRD
#define UC_LED_PORT		PORTD
#define UC_LED_PIN		PD5
#define CLOCK_LED_DDR	DDRD
#define CLOCK_LED_PORT	PORTD
#define CLOCK_LED_PIN	PD6
#define TEST1_LED_DDR	DDRD
#define TEST1_LED_PORT	PORTD
#define TEST1_LED_PIN	PD7


/********* BUTTONS ********/
#define BUTTON_OFF_DDR		DDRC
#define BUTTON_OFF_PORT		PINC
#define BUTTON_OFF_PIN		BUTTON_INVALID
#define BUTTON_MODE_DDR		DDRC
#define BUTTON_MODE_PORT	PINC
#define BUTTON_MODE_PIN		PC2
#define BUTTON_FUNC1_DDR	DDRC
#define BUTTON_FUNC1_PORT	PINC
#define BUTTON_FUNC1_PIN	BUTTON_INVALID
#define BUTTON_FUNC2_DDR	DDRC
#define BUTTON_FUNC2_PORT	PINC
#define BUTTON_FUNC2_PIN	PC4
#define BUTTON_FUNC3_DDR	DDRC
#define BUTTON_FUNC3_PORT	PINC
#define BUTTON_FUNC3_PIN	BUTTON_INVALID
#define BUTTON_LEFT_DDR		DDRC
#define BUTTON_LEFT_PORT	PINC
#define BUTTON_LEFT_PIN		BUTTON_INVALID
#define BUTTON_RIGHT_DDR	DDRC
#define BUTTON_RIGHT_PORT	PINC
#define BUTTON_RIGHT_PIN	BUTTON_INVALID
#define BUTTON_UP_DDR		DDRC
#define BUTTON_UP_PORT		PINC
#define BUTTON_UP_PIN		BUTTON_INVALID
#define BUTTON_DOWN_DDR		DDRC
#define BUTTON_DOWN_PORT	PINC
#define BUTTON_DOWN_PIN		BUTTON_INVALID


/********** USART *********/
#define USART_REQESTER_TRANSFER				0x10
#define USART_DATA_LENGTH_TRANSFER_READ		1
#define USART_DATA_LENGTH_TRANSFER_WRITE	20

#define USART_DATA_LENGTH_READ_MAX			10


/*********** LCD **********/
#define LCD_RS_DDR					DDRB
#define LCD_RS_PORT					PORTB
#define LCD_RS_PIN					PB0

#define LCD_RW_DDR					DDRB
#define LCD_RW_PORT					PORTB
#define LCD_RW_PIN					PB1

#define LCD_E_DDR					DDRB
#define LCD_E_PORT					PORTB
#define LCD_E_PIN					PB2

#define LCD_DB_DDR					DDRA
#define LCD_DB_PORT					PORTA
#define LCD_DB0_PIN					PA0
#define LCD_DB1_PIN					PA1
#define LCD_DB2_PIN					PA2
#define LCD_DB3_PIN					PA3
#define LCD_DB4_PIN					PA4
#define LCD_DB5_PIN					PA5
#define LCD_DB6_PIN					PA6
#define LCD_DB7_PIN					PA7


/*********** DS18B20 **********/
#define DS18B20_DATA_DDR			DDRC
#define DS18B20_DATA_PORT			PORTC
#define DS18B20_DATA_PINR			PINC
#define DS18B20_DATA_PIN			PC5

#define DS18B20_ALARM_TLOW			0	/* not supported for now */
#define DS18B20_ALARM_THIGH			0	/* not supported for now */

/* SPI not needed but speed has to be defined */
#define SPI_SPEED SPI_DISABLED

#endif

#endif /* TEMPERATURELOGGER_DRIVERS_CONFIG_H_ */
