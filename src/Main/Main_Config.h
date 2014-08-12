/*
 * Main_Config.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_


/* size of the LED matrix */
#define LCD_MATRIX_SIZE_LIN				4
#define LCD_MATRIX_SIZE_COL				20

/* clock */
#define CLOCK_TYPE_OFF					1
#define CLOCK_TYPE_DS1307				2
#define CLOCK_TYPE						CLOCK_TYPE_DS1307

/* clock sync */
#define CLOCK_SYNC_OFF					1
#define CLOCK_SYNC_DCF77				2
#define CLOCK_SYNC_USART				3
#define CLOCK_SYNC						CLOCK_SYNC_OFF

/* buttons */
#define BUTTONS_MULTIPLEX_OFF			1
#define BUTTONS_MULTIPLEX_HC165			2
#define BUTTONS_MULTIPLEX				BUTTONS_MULTIPLEX_OFF

#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_USART					BUTTONS_USART_OFF

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2
#define LCD_CONTROLLER					LCD_CONTROLLER_SPLC780D1

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_DS18B20		2
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_DS18B20

/* measurement */
/* TODO: configure resolution */
#define MAX_MEASUREMENT_POINTS			(1900 /2)

#endif /* MAIN_CONFIG_H_ */
