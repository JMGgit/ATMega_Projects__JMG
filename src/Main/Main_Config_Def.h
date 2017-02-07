/*
 * Main_Config_def.h
 *
 *  Created on: 09.04.2016
 *      Author: Jean-Martin George
 */

#ifndef MAIN_CONFIG_DEF_H_
#define MAIN_CONFIG_DEF_H_


/* projects */
#define PROJECT__TEMPERATURE_LOGGER		1
#define	PROJECT__QLOCKTWO				2
#define PROJECT__LED_TABLE				3
#define PROJECT__LED_MIRROR				4
#define PROJECT__LED_FRAME				5
#define PROJECT__IRMP					6

/* Qlocktwo revsion */
#define QLOCKTWO_REVISION_3				1
#define QLOCKTWO_REVISION_4				2

/* LED Table revsion */
#define LEDTABLE_REVISION_1				1
#define LEDTABLE_REVISION_2				2

/* clock */
#define CLOCK_TYPE_OFF					1
#define CLOCK_TYPE_DS1307				2

/* clock sync */
#define CLOCK_SYNC_OFF					1
#define CLOCK_SYNC_DCF77				2
#define CLOCK_SYNC_USART				3

/* buttons */
#define BUTTONS_WIRED_OFF				1
#define BUTTONS_WIRED_PIN				2
#define BUTTONS_WIRED_HC165				3
#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_IRMP_OFF				1
#define BUTTONS_IRMP_USED				2
#define BUTTONS_IRMP_SEND_TO_TWI		3
#define BUTTONS_TWI_OFF					1
#define BUTTONS_TWI_ON					2

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_TEST			2
#define TEMPERATURE_SENSOR_DS18B20		3

/* runtime test */
#define RUNTIME_TEST_OFF				1
#define RUNTIME_TEST_ON					2

/* debug mode */
#define DEBUG_MODE_OFF					1
#define DEBUG_MODE_ON					2

/* snake game */
#define MODE_SNAKE_OFF					1
#define MODE_SNAKE_ON					2

/* LED colors */
#define LED_COLORS_PWM					1
#define LED_COLORS_LINEAR				2


#endif /* MAIN_CONFIG_DEF_H_ */
