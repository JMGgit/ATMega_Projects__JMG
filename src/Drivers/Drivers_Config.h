/*
 * Drivers_Config.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef DRIVERS_CONFIG_H_
#define DRIVERS_CONFIG_H_

#include "Main_Config.h"


#if (PROJECT == PROJECT__TEMPERATURE_LOGGER)

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

/********** IRMP **********/
#define IRMP_OFF		0
#define IRMP_ON			1
#define IRMP			IRMP_OFF

#endif


#if (PROJECT == PROJECT__QLOCKTWO_2_0)

/*********** WS2801 ***********/
#define WS2801_NB		114
#define RGB_LED_CONNECTION__RED_GREEN_BLUE	1
#define RGB_LED_CONNECTION__BLUE_GREEN_RED	2
#define RGB_LED_CONNECTION					RGB_LED_CONNECTION__RED_GREEN_BLUE

/********** DCF77 ***************/
#define DCF77_DATA_DDR	DDRB
#define DCF77_DATA_PORT	PINB
#define DCF77_DATA_PIN	PB0
#define DCF77_TASK_TIME	100
/* 16 Mhz -> 125 */
/* 20 Mhz -> 100 */

/*********** CTL LED ***********/
#define UC_LED_DDR		DDRD
#define UC_LED_PORT		PORTD
#define UC_LED_PIN		PD4
#define CLOCK_LED_DDR	DDRD
#define CLOCK_LED_PORT	PORTD
#define CLOCK_LED_PIN	PD5
#define TEST1_LED_DDR	DDRD
#define TEST1_LED_PORT	PORTD
#define TEST1_LED_PIN	PD6
#define DCF77_LED_DDR	TEST1_LED_DDR
#define DCF77_LED_PORT	TEST1_LED_PORT
#define DCF77_LED_PIN	TEST1_LED_PIN

/********** USART *********/
#define USART_DATA_LENGTH_READ_MAX	0

/********* 74HC165 ********/
#define HC165_CLK_DDR	DDRB
#define HC165_CLK_PORT	PORTB
#define HC165_CLK_PIN	PB3
#define HC165_SL_DDR	DDRB
#define HC165_SL_PORT	PORTB
#define HC165_SL_PIN	PB2
#define HC165_OUT_DDR	DDRB
#define HC165_OUT_PORT	PINB
#define HC165_OUT_PIN	PB1

/********** IRMP **********/
#define IRMP_OFF		0
#define IRMP_ON			1
#define IRMP			IRMP_OFF

#endif


#if (PROJECT == PROJECT__QLOCKTWO_3_0)

/*********** WS2801 ***********/
#define WS2801_NB		114
#define RGB_LED_CONNECTION__RED_GREEN_BLUE	1
#define RGB_LED_CONNECTION__BLUE_GREEN_RED	2
#define RGB_LED_CONNECTION					RGB_LED_CONNECTION__RED_GREEN_BLUE

/********** DCF77 ***************/
#define DCF77_DATA_DDR	DDRB
#define DCF77_DATA_PORT	PINB
#define DCF77_DATA_PIN	PB0
#define DCF77_TASK_TIME	100
/* 16 Mhz -> 125 */
/* 20 Mhz -> 100 */

/*********** CTL LED ***********/
#define UC_LED_DDR		DDRD
#define UC_LED_PORT		PORTD
#define UC_LED_PIN		PD4
#define CLOCK_LED_DDR	DDRD
#define CLOCK_LED_PORT	PORTD
#define CLOCK_LED_PIN	PD5
#define TEST1_LED_DDR	DDRD
#define TEST1_LED_PORT	PORTD
#define TEST1_LED_PIN	PD6
#define TEST2_LED_DDR	DDRD
#define TEST2_LED_PORT	PORTD
#define TEST2_LED_PIN	PD7
#define DCF77_LED_DDR	TEST1_LED_DDR
#define DCF77_LED_PORT	TEST1_LED_PORT
#define DCF77_LED_PIN	TEST1_LED_PIN
#define IRMP_LED_DDR	TEST2_LED_DDR
#define IRMP_LED_PORT	TEST2_LED_PORT
#define IRMP_LED_PIN	TEST2_LED_PIN

/********* BUTTONS ********/
#define BUTTON_MODE_DDR		DDRB
#define BUTTON_MODE_PORT	PINB
#define BUTTON_MODE_PIN		PB1
#define BUTTON_FUNC1_DDR	DDRB
#define BUTTON_FUNC1_PORT	PINB
#define BUTTON_FUNC1_PIN	PB2
#define BUTTON_FUNC2_DDR	DDRB
#define BUTTON_FUNC2_PORT	PINB
#define BUTTON_FUNC2_PIN	PB3
#define BUTTON_FUNC3_DDR	DDRB
#define BUTTON_FUNC3_PORT	PINB
#define BUTTON_FUNC3_PIN	BUTTON_INVALID
#define BUTTON_LEFT_DDR		DDRB
#define BUTTON_LEFT_PORT	PINB
#define BUTTON_LEFT_PIN		BUTTON_INVALID
#define BUTTON_RIGHT_DDR	DDRB
#define BUTTON_RIGHT_PORT	PINB
#define BUTTON_RIGHT_PIN	BUTTON_INVALID
#define BUTTON_UP_DDR		DDRB
#define BUTTON_UP_PORT		PINB
#define BUTTON_UP_PIN		BUTTON_INVALID
#define BUTTON_DOWN_DDR		DDRB
#define BUTTON_DOWN_PORT	PINB
#define BUTTON_DOWN_PIN		BUTTON_INVALID

/********** USART *********/
#define USART_DATA_LENGTH_READ_MAX	0

/********** IRMP **********/
#define IRMP_OFF		0
#define IRMP_ON			1
#define IRMP			IRMP_ON

#define IRMP_BUTTON_MODE		0x20DF
#define IRMP_BUTTON_FUNC1		0xF807
#define IRMP_BUTTON_FUNC2		0x7887
#define IRMP_BUTTON_FUNC3		0x58A7
#define IRMP_BUTTON_LEFT		0x10EF
#define IRMP_BUTTON_UP			0xA05F
#define IRMP_BUTTON_DOWN		0x00FF
#define IRMP_BUTTON_RIGHT		0x807F

#endif


#if (PROJECT == PROJECT__LED_TABLE)

/*********** WS2801 ***********/
#define WS2801_NB		(LED_MATRIX_SIZE_COL * LED_MATRIX_SIZE_LIN)
#define RGB_LED_CONNECTION__RED_GREEN_BLUE	1
#define RGB_LED_CONNECTION__BLUE_GREEN_RED	2
#define RGB_LED_CONNECTION					RGB_LED_CONNECTION__BLUE_GREEN_RED

/********** DCF77 ***************/
#define DCF77_DATA_DDR	DDRB
#define DCF77_DATA_PORT	PINB
#define DCF77_DATA_PIN	PB0
#define DCF77_TASK_TIME	125
/* 16 Mhz -> 125 */
/* 20 Mhz -> 100 */

/*********** CTL LED ***********/
#define UC_LED_DDR		DDRD
#define UC_LED_PORT		PORTD
#define UC_LED_PIN		PD4
#define CLOCK_LED_DDR	DDRD
#define CLOCK_LED_PORT	PORTD
#define CLOCK_LED_PIN	PD5
#define TEST1_LED_DDR	DDRD
#define TEST1_LED_PORT	PORTD
#define TEST1_LED_PIN	PD7


/********* 74HC165 ********/
#define HC165_CLK_DDR	DDRB
#define HC165_CLK_PORT	PORTB
#define HC165_CLK_PIN	PB3
#define HC165_SL_DDR	DDRB
#define HC165_SL_PORT	PORTB
#define HC165_SL_PIN	PB2
#define HC165_OUT_DDR	DDRB
#define HC165_OUT_PORT	PINB
#define HC165_OUT_PIN	PB1


/********** USART *********/

#define USART_REQESTER_MODE			0x40
#define USART_REQESTER_BUTTON		0x30
#define USART_REQESTER_COLOR		0x10
#define USART_REQESTER_CLOCK		0x80

#define USART_DATA_LENGTH_MODE		1
#define USART_DATA_LENGTH_BUTTON	2
#define USART_DATA_LENGTH_COLOR		4
#define USART_DATA_LENGTH_CLOCK		8
#define USART_DATA_LENGTH_READ_MAX	4

/********** IRMP **********/
#define IRMP_OFF		0
#define IRMP_ON			1
#define IRMP			IRMP_OFF

#endif

#endif /* DRIVERS_CONFIG_H_ */
