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
#define QLOCKTWO_REVISION_2				1
#define QLOCKTWO_REVISION_3				2
#define QLOCKTWO_REVISION_4				3

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
#define BUTTON_OFF_AVAILABLE_NO			1
#define BUTTON_OFF_AVAILABLE_YES		2
#define BUTTON_OFF_AVAILABLE_FUNC2		3
#define BUTTON_FUNC3_AVAILABLE_NO		1
#define BUTTON_FUNC3_AVAILABLE_YES		2

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

/* qlocktwo language */
#define QLOCKTWO_LANG_DE				1
#define QLOCKTWO_LANG_DE_SUED			2
#define QLOCKTWO_LANG_FR				3
#define QLOCKTWO_LANG_EN				4
#define QLOCKTWO_LANG_MULTI				5

/* snake game */
#define MODE_SNAKE_OFF					1
#define MODE_SNAKE_ON					2


#endif /* MAIN_CONFIG_DEF_H_ */