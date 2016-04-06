/*
 * Main_Config.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_


#define PROJECT__TEMPERATURE_LOGGER		1
#define	PROJECT__QLOCKTWO				2
#define PROJECT__LED_TABLE				3
#define PROJECT__LED_MIRROR				4
#define PROJECT__LED_FRAME				5
#define PROJECT__IRMP					6


#if (PROJECT == PROJECT__TEMPERATURE_LOGGER)

/* size of the LCD matrix */
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
#define BUTTONS_WIRED_OFF				1
#define BUTTONS_WIRED_PIN				2
#define BUTTONS_WIRED_HC165				3
#define BUTTONS_WIRED					BUTTONS_WIRED_PIN

#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_USART					BUTTONS_USART_OFF

#define BUTTONS_IRMP_OFF				1
#define BUTTONS_IRMP_USED				2
#define BUTTONS_IRMP_SEND_TO_TWI		3
#define BUTTONS_IRMP					BUTTONS_IRMP_OFF

#define BUTTONS_TWI_OFF					1
#define BUTTONS_TWI_ON					2
#define BUTTONS_TWI						BUTTONS_TWI_OFF

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2
#define LCD_CONTROLLER					LCD_CONTROLLER_SPLC780D1

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_TEST			2
#define TEMPERATURE_SENSOR_DS18B20		3
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_TEST

/* measurement */
/* TODO: configure resolution */
#define MAX_MEASUREMENT_POINTS			(1900 /2)

#endif


#if (PROJECT == PROJECT__QLOCKTWO)
#ifdef __AVR_ATmega644P__ /* Qlocktwo 2.x / 3.0 */

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2
#define LCD_CONTROLLER					LCD_CONTROLLER_OFF

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_TEST			2
#define TEMPERATURE_SENSOR_DS18B20		3
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_OFF

/* clock */
#define CLOCK_TYPE_OFF					1
#define CLOCK_TYPE_DS1307				2
#define CLOCK_TYPE						CLOCK_TYPE_DS1307

/* size of the LED matrix */
#define LED_MATRIX_SIZE_LIN				10
#define LED_MATRIX_SIZE_COL				11

/* dot correction for LEDs (white color) */
#define WHITE_COLOR_PERCENT_RED			70
#define WHITE_COLOR_PERCENT_GREEN		100
#define WHITE_COLOR_PERCENT_BLUE		100

/* language */
#define QLOCKTWO_LANG_DE				1
#define QLOCKTWO_LANG_DE_SUED			2
#define QLOCKTWO_LANG_FR				3
#define QLOCKTWO_LANG_EN				4
#define QLOCKTWO_LANG_MULTI				5
#define QLOCKTWO_LANG					QLOCKTWO_LANG_MULTI

/* clock sync */
#define CLOCK_SYNC_OFF					1
#define CLOCK_SYNC_DCF77				2
#define CLOCK_SYNC_USART				3
#define CLOCK_SYNC						CLOCK_SYNC_DCF77

/* buttons */
#define BUTTONS_WIRED_OFF				1
#define BUTTONS_WIRED_PIN				2
#define BUTTONS_WIRED_HC165				3
#define BUTTONS_WIRED					BUTTONS_WIRED_PIN

#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_USART					BUTTONS_USART_OFF

#define BUTTONS_IRMP_OFF				1
#define BUTTONS_IRMP_USED				2
#define BUTTONS_IRMP_SEND_TO_TWI		3
#define BUTTONS_IRMP					BUTTONS_IRMP_USED

#define BUTTONS_TWI_OFF					1
#define BUTTONS_TWI_ON					2
#define BUTTONS_TWI						BUTTONS_TWI_OFF

/* button for off mode */
#define BUTTON_OFF_AVAILABLE_NO			1
#define BUTTON_OFF_AVAILABLE_YES		2
#define BUTTON_OFF_AVAILABLE_FUNC2		3
/* defined by project configuration */

/* availability of button func3 */
#define BUTTON_FUNC3_AVAILABLE_NO		1
#define BUTTON_FUNC3_AVAILABLE_YES		2
/* defined by project configuration */

/* startup mode */
#define MODE_STARTUP_OFF				1
#define MODE_STARTUP_ON					2
#define MODE_STARTUP					MODE_STARTUP_ON

/* snake game */
#define MODE_SNAKE_OFF					1
#define MODE_SNAKE_ON					2
/* defined by project configuration */

/* brightness */
#define QTWO_BRIGHTNESS_LEVEL_1				4
#define QTWO_BRIGHTNESS_LEVEL_2				10
#define QTWO_BRIGHTNESS_LEVEL_3				25
#define QTWO_BRIGHTNESS_LEVEL_4				90
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_1		10
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_2		25
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_3		90
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_4		160
#define SNAKE_BRIGHTNESS_LEVEL				30
#define TEST_BRIGHTNESS_LEVEL				60

/* runtime test */
#define RUNTIME_TEST_OFF					1
#define RUNTIME_TEST_ON						2
#define RUNTIME_TEST						RUNTIME_TEST_OFF

#endif
#endif


#if (PROJECT == PROJECT__QLOCKTWO)
#ifdef __AVR_ATmega328P__ /* Qlocktwo 4.0 */

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2
#define LCD_CONTROLLER					LCD_CONTROLLER_OFF

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_TEST			2
#define TEMPERATURE_SENSOR_DS18B20		3
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_OFF

/* clock */
#define CLOCK_TYPE_OFF					1
#define CLOCK_TYPE_DS1307				2
#define CLOCK_TYPE						CLOCK_TYPE_DS1307

/* size of the LED matrix */
#define LED_MATRIX_SIZE_LIN				10
#define LED_MATRIX_SIZE_COL				11

/* dot correction for LEDs (white color) */
#define WHITE_COLOR_PERCENT_RED			70
#define WHITE_COLOR_PERCENT_GREEN		100
#define WHITE_COLOR_PERCENT_BLUE		100

/* language */
#define QLOCKTWO_LANG_DE				1
#define QLOCKTWO_LANG_DE_SUED			2
#define QLOCKTWO_LANG_FR				3
#define QLOCKTWO_LANG_EN				4
#define QLOCKTWO_LANG_MULTI				5
#define QLOCKTWO_LANG					QLOCKTWO_LANG_MULTI

/* clock sync */
#define CLOCK_SYNC_OFF					1
#define CLOCK_SYNC_DCF77				2
#define CLOCK_SYNC_USART				3
#define CLOCK_SYNC						CLOCK_SYNC_DCF77

/* buttons */
#define BUTTONS_WIRED_OFF				1
#define BUTTONS_WIRED_PIN				2
#define BUTTONS_WIRED_HC165				3
#define BUTTONS_WIRED					BUTTONS_WIRED_OFF

#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_USART					BUTTONS_USART_OFF

#define BUTTONS_IRMP_OFF				1
#define BUTTONS_IRMP_USED				2
#define BUTTONS_IRMP_SEND_TO_TWI		3
#define BUTTONS_IRMP					BUTTONS_IRMP_USED

#define BUTTONS_TWI_OFF					1
#define BUTTONS_TWI_ON					2
#define BUTTONS_TWI						BUTTONS_TWI_OFF

/* button for off mode */
#define BUTTON_OFF_AVAILABLE_NO			1
#define BUTTON_OFF_AVAILABLE_YES		2
#define BUTTON_OFF_AVAILABLE_FUNC2		3
#define BUTTON_OFF_AVAILABLE			BUTTON_OFF_AVAILABLE_YES

/* availability of button func3 */
#define BUTTON_FUNC3_AVAILABLE_NO		1
#define BUTTON_FUNC3_AVAILABLE_YES		2
#define BUTTON_FUNC3_AVAILABLE			BUTTON_FUNC3_AVAILABLE_YES

/* startup mode */
#define MODE_STARTUP_OFF				1
#define MODE_STARTUP_ON					2
#define MODE_STARTUP					MODE_STARTUP_ON

/* snake game */
#define MODE_SNAKE_OFF					1
#define MODE_SNAKE_ON					2
#define MODE_SNAKE						MODE_SNAKE_ON

/* brightness */
#define QTWO_BRIGHTNESS_LEVEL_1				4
#define QTWO_BRIGHTNESS_LEVEL_2				10
#define QTWO_BRIGHTNESS_LEVEL_3				25
#define QTWO_BRIGHTNESS_LEVEL_4				90
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_1		10
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_2		25
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_3		90
#define QTWO_BRIGHTNESS_LEVEL_BRIGHT_4		160
#define SNAKE_BRIGHTNESS_LEVEL				30
#define TEST_BRIGHTNESS_LEVEL				60

/* runtime test */
#define RUNTIME_TEST_OFF					1
#define RUNTIME_TEST_ON						2
#define RUNTIME_TEST						RUNTIME_TEST_OFF

#endif
#endif


#if (PROJECT == PROJECT__IRMP)

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2
#define LCD_CONTROLLER					LCD_CONTROLLER_OFF

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_TEST			2
#define TEMPERATURE_SENSOR_DS18B20		3
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_OFF

/* clock */
#define CLOCK_TYPE_OFF					1
#define CLOCK_TYPE_DS1307				2
#define CLOCK_TYPE						CLOCK_TYPE_OFF

/* clock sync */
#define CLOCK_SYNC_OFF					1
#define CLOCK_SYNC_DCF77				2
#define CLOCK_SYNC_USART				3
#define CLOCK_SYNC						CLOCK_SYNC_OFF

/* buttons */
#define BUTTONS_WIRED_OFF				1
#define BUTTONS_WIRED_PIN				2
#define BUTTONS_WIRED_HC165				3
#define BUTTONS_WIRED					BUTTONS_WIRED_OFF

#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_USART					BUTTONS_USART_OFF

#define BUTTONS_IRMP_OFF				1
#define BUTTONS_IRMP_USED				2
#define BUTTONS_IRMP_SEND_TO_TWI		3
#define BUTTONS_IRMP					BUTTONS_IRMP_SEND_TO_TWI

#define BUTTONS_TWI_OFF					1
#define BUTTONS_TWI_ON					2
#define BUTTONS_TWI						BUTTONS_TWI_OFF

/* button for off mode */
#define BUTTON_OFF_AVAILABLE_NO			1
#define BUTTON_OFF_AVAILABLE_YES		2
#define BUTTON_OFF_AVAILABLE_FUNC2		3
#define BUTTON_OFF_AVAILABLE			BUTTON_OFF_AVAILABLE_YES

/* startup mode */
#define MODE_STARTUP_OFF				1
#define MODE_STARTUP_ON					2
#define MODE_STARTUP					MODE_STARTUP_OFF

/* snake game */
#define MODE_SNAKE_OFF					1
#define MODE_SNAKE_ON					2
#define MODE_SNAKE						MODE_SNAKE_OFF


/* runtime test */
#define RUNTIME_TEST_OFF				1
#define RUNTIME_TEST_ON					2
#define RUNTIME_TEST					RUNTIME_TEST_OFF

#endif


#if (PROJECT == PROJECT__LED_TABLE)

/* size of the LED matrix */
#define LED_MATRIX_SIZE_LIN				13
#define LED_MATRIX_SIZE_COL				23

/* clock */
#define CLOCK_TYPE_OFF					1
#define CLOCK_TYPE_DS1307				2
#define CLOCK_TYPE						CLOCK_TYPE_DS1307

/* dot correction for LEDs (white color) */
#define WHITE_COLOR_PERCENT_RED			70
#define WHITE_COLOR_PERCENT_GREEN		100
#define WHITE_COLOR_PERCENT_BLUE		100

/* clock sync */
#define CLOCK_SYNC_OFF					1
#define CLOCK_SYNC_DCF77				2
#define CLOCK_SYNC_USART				3
#define CLOCK_SYNC						CLOCK_SYNC_DCF77

/* buttons */
#define BUTTONS_WIRED_OFF				1
#define BUTTONS_WIRED_PIN				2
#define BUTTONS_WIRED_HC165				3
#define BUTTONS_WIRED					BUTTONS_WIRED_OFF

#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_USART					BUTTONS_USART_ON

#define BUTTONS_IRMP_OFF				1
#define BUTTONS_IRMP_USED				2
#define BUTTONS_IRMP_SEND_TO_TWI		3
#define BUTTONS_IRMP					BUTTONS_IRMP_USED

#define BUTTONS_TWI_OFF					1
#define BUTTONS_TWI_ON					2
#define BUTTONS_TWI						BUTTONS_TWI_OFF

/* button for off mode */
#define BUTTON_OFF_AVAILABLE_NO			1
#define BUTTON_OFF_AVAILABLE_YES		2
#define BUTTON_OFF_AVAILABLE_FUNC2		3
/* defined by project configuration */

/* availability of button func3 */
#define BUTTON_FUNC3_AVAILABLE_NO		1
#define BUTTON_FUNC3_AVAILABLE_YES		2
/* defined by project configuration */

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2
#define LCD_CONTROLLER					LCD_CONTROLLER_OFF

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_TEST			2
#define TEMPERATURE_SENSOR_DS18B20		3
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_OFF

/* startup mode */
#define MODE_STARTUP_OFF				1
#define MODE_STARTUP_ON					2
#define MODE_STARTUP					MODE_STARTUP_OFF

/* snake game */
#define MODE_SNAKE_OFF					1
#define MODE_SNAKE_ON					2

/* brightness */
#define SNAKE_BRIGHTNESS_LEVEL			255
#define TEST_BRIGHTNESS_LEVEL			60

/* runtime test */
#define RUNTIME_TEST_OFF				1
#define RUNTIME_TEST_ON					2
#define RUNTIME_TEST					RUNTIME_TEST_OFF

#endif


#if (PROJECT == PROJECT__LED_MIRROR)

/* size of the LED matrix */
#define LED_MATRIX_SIZE_LIN				2
#define LED_MATRIX_SIZE_COL				40

/* clock */
#define CLOCK_TYPE_OFF					1
#define CLOCK_TYPE_DS1307				2
#define CLOCK_TYPE						CLOCK_TYPE_OFF

/* dot correction for LEDs (white color) */
#define WHITE_COLOR_PERCENT_RED			70
#define WHITE_COLOR_PERCENT_GREEN		100
#define WHITE_COLOR_PERCENT_BLUE		100

/* clock sync */
#define CLOCK_SYNC_OFF					1
#define CLOCK_SYNC_DCF77				2
#define CLOCK_SYNC_USART				3
#define CLOCK_SYNC						CLOCK_SYNC_OFF

/* buttons */
#define BUTTONS_WIRED_OFF				1
#define BUTTONS_WIRED_PIN				2
#define BUTTONS_WIRED_HC165				3
#define BUTTONS_WIRED					BUTTONS_WIRED_PIN

#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_USART					BUTTONS_USART_ON

#define BUTTONS_IRMP_OFF				1
#define BUTTONS_IRMP_USED				2
#define BUTTONS_IRMP_SEND_TO_TWI		3
#define BUTTONS_IRMP					BUTTONS_IRMP_USED

#define BUTTONS_TWI_OFF					1
#define BUTTONS_TWI_ON					2
#define BUTTONS_TWI						BUTTONS_TWI_OFF

/* button for off mode */
#define BUTTON_OFF_AVAILABLE_NO			1
#define BUTTON_OFF_AVAILABLE_YES		2
#define BUTTON_OFF_AVAILABLE_FUNC2		3
/* defined by project configuration */

/* availability of button func3 */
#define BUTTON_FUNC3_AVAILABLE_NO		1
#define BUTTON_FUNC3_AVAILABLE_YES		2
/* defined by project configuration */

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2
#define LCD_CONTROLLER					LCD_CONTROLLER_OFF

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_TEST			2
#define TEMPERATURE_SENSOR_DS18B20		3
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_OFF

/* startup mode */
#define MODE_STARTUP_OFF				1
#define MODE_STARTUP_ON					2
#define MODE_STARTUP					MODE_STARTUP_OFF

/* snake game */
#define MODE_SNAKE_OFF					1
#define MODE_SNAKE_ON					2

/* brightness */
#define TEST_BRIGHTNESS_LEVEL			60

/* runtime test */
#define RUNTIME_TEST_OFF				1
#define RUNTIME_TEST_ON					2
#define RUNTIME_TEST					RUNTIME_TEST_OFF

#endif


#if (PROJECT == PROJECT__LED_FRAME)

/* size of the LED matrix */
#define LED_MATRIX_SIZE_LIN				2
#define LED_MATRIX_SIZE_COL				40

/* clock */
#define CLOCK_TYPE_OFF					1
#define CLOCK_TYPE_DS1307				2
#define CLOCK_TYPE						CLOCK_TYPE_OFF

/* dot correction for LEDs (white color) */
#define WHITE_COLOR_PERCENT_RED			70
#define WHITE_COLOR_PERCENT_GREEN		100
#define WHITE_COLOR_PERCENT_BLUE		100

/* clock sync */
#define CLOCK_SYNC_OFF					1
#define CLOCK_SYNC_DCF77				2
#define CLOCK_SYNC_USART				3
#define CLOCK_SYNC						CLOCK_SYNC_OFF

/* buttons */
#define BUTTONS_WIRED_OFF				1
#define BUTTONS_WIRED_PIN				2
#define BUTTONS_WIRED_HC165				3
#define BUTTONS_WIRED					BUTTONS_WIRED_OFF

#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_USART					BUTTONS_USART_ON

#define BUTTONS_IRMP_OFF				1
#define BUTTONS_IRMP_USED				2
#define BUTTONS_IRMP_SEND_TO_TWI		3
#define BUTTONS_IRMP					BUTTONS_IRMP_USED

#define BUTTONS_TWI_OFF					1
#define BUTTONS_TWI_ON					2
#define BUTTONS_TWI						BUTTONS_TWI_OFF

/* button for off mode */
#define BUTTON_OFF_AVAILABLE_NO			1
#define BUTTON_OFF_AVAILABLE_YES		2
#define BUTTON_OFF_AVAILABLE_FUNC2		3
/* defined by project configuration */

/* availability of button func3 */
#define BUTTON_FUNC3_AVAILABLE_NO		1
#define BUTTON_FUNC3_AVAILABLE_YES		2
/* defined by project configuration */

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2
#define LCD_CONTROLLER					LCD_CONTROLLER_OFF

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_TEST			2
#define TEMPERATURE_SENSOR_DS18B20		3
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_OFF

/* startup mode */
#define MODE_STARTUP_OFF				1
#define MODE_STARTUP_ON					2
#define MODE_STARTUP					MODE_STARTUP_OFF

/* snake game */
#define MODE_SNAKE_OFF					1
#define MODE_SNAKE_ON					2

/* brightness */
#define TEST_BRIGHTNESS_LEVEL			60

/* runtime test */
#define RUNTIME_TEST_OFF				1
#define RUNTIME_TEST_ON					2
#define RUNTIME_TEST					RUNTIME_TEST_OFF

#endif

#endif /* MAIN_CONFIG_H_ */
