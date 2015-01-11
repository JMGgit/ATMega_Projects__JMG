/*
 * Main_Config.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_


#define PROJECT__TEMPERATURE_LOGGER		1
#define	PROJECT__QLOCKTWO_2_0			2
#define	PROJECT__QLOCKTWO_3_0			3
#define PROJECT__LED_TABLE				4


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
#define BUTTONS_IRMP_ON					2
#define BUTTONS_IRMP					BUTTONS_IRMP_OFF

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2
#define LCD_CONTROLLER					LCD_CONTROLLER_SPLC780D1

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_TEST			2
#define TEMPERATURE_SENSOR_DS18B20		3
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_DS18B20

/* measurement */
/* TODO: configure resolution */
#define MAX_MEASUREMENT_POINTS			(1900 /2)

#endif


#if (PROJECT == PROJECT__QLOCKTWO_2_0)

/* size of the LED matrix */
#define LED_MATRIX_SIZE_LIN				10
#define LED_MATRIX_SIZE_COL				11

/* clock */
#define CLOCK_TYPE_OFF					1
#define CLOCK_TYPE_DS1307				2
#define CLOCK_TYPE						CLOCK_TYPE_DS1307

/* dot correction for LEDs (white color) */
#define WHITE_COLOR_PERCENT_RED			70
#define WHITE_COLOR_PERCENT_GREEN		100
#define WHITE_COLOR_PERCENT_BLUE		100

/* language */
#define QLOCKTWO_LANG_DE				1
#define QLOCKTWO_LANG_DE_SUED			2
#define QLOCKTWO_LANG_FR				3
#define QLOCKTWO_LANG					QLOCKTWO_LANG_DE_SUED

/* clock sync */
#define CLOCK_SYNC_OFF					1
#define CLOCK_SYNC_DCF77				2
#define CLOCK_SYNC_USART				3
#define CLOCK_SYNC						CLOCK_SYNC_DCF77

/* buttons */
#define BUTTONS_WIRED_OFF				1
#define BUTTONS_WIRED_PIN				2
#define BUTTONS_WIRED_HC165				3
#define BUTTONS_WIRED					BUTTONS_WIRED_HC165

#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_USART					BUTTONS_USART_OFF

#define BUTTONS_IRMP_OFF				1	
#define BUTTONS_IRMP_ON					2
#define BUTTONS_IRMP					BUTTONS_IRMP_OFF

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
#define MODE_STARTUP					MODE_STARTUP_ON

/* button for off mode */
#define OFF_BUTTON_MODE					1
#define OFF_BUTTON_FUNC2				2
#define OFF_BUTTON_OFF					3
#define OFF_BUTTON						OFF_BUTTON_MODE

/* brightness */
#define BRIGHTNESS_LEVEL_1				4
#define BRIGHTNESS_LEVEL_2				10
#define BRIGHTNESS_LEVEL_3				25
#define BRIGHTNESS_LEVEL_4				90
#define BRIGHTNESS_LEVEL_BRIGHT_1		10
#define BRIGHTNESS_LEVEL_BRIGHT_2		25
#define BRIGHTNESS_LEVEL_BRIGHT_3		90
#define BRIGHTNESS_LEVEL_BRIGHT_4		160

#endif


#if (PROJECT == PROJECT__QLOCKTWO_3_0)

/* size of the LED matrix */
#define LED_MATRIX_SIZE_LIN				10
#define LED_MATRIX_SIZE_COL				11

/* clock */
#define CLOCK_TYPE_OFF					1
#define CLOCK_TYPE_DS1307				2
#define CLOCK_TYPE						CLOCK_TYPE_DS1307

/* dot correction for LEDs (white color) */
#define WHITE_COLOR_PERCENT_RED			70
#define WHITE_COLOR_PERCENT_GREEN		100
#define WHITE_COLOR_PERCENT_BLUE		100

/* language */
#define QLOCKTWO_LANG_DE				1
#define QLOCKTWO_LANG_DE_SUED			2
#define QLOCKTWO_LANG_FR				3
#define QLOCKTWO_LANG					QLOCKTWO_LANG_DE_SUED

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
#define BUTTONS_IRMP_ON					2
#define BUTTONS_IRMP					BUTTONS_IRMP_ON

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
#define MODE_STARTUP					MODE_STARTUP_ON

/* button for off mode */
#define OFF_BUTTON_MODE					1
#define OFF_BUTTON_FUNC2				2
#define OFF_BUTTON_OFF					3
#define OFF_BUTTON						OFF_BUTTON_OFF

/* brightness */
#define BRIGHTNESS_LEVEL_1				4
#define BRIGHTNESS_LEVEL_2				10
#define BRIGHTNESS_LEVEL_3				25
#define BRIGHTNESS_LEVEL_4				90
#define BRIGHTNESS_LEVEL_BRIGHT_1		10
#define BRIGHTNESS_LEVEL_BRIGHT_2		25
#define BRIGHTNESS_LEVEL_BRIGHT_3		90
#define BRIGHTNESS_LEVEL_BRIGHT_4		160

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
#define CLOCK_SYNC						CLOCK_SYNC_OFF

/* buttons */
#define BUTTONS_WIRED_OFF				1
#define BUTTONS_WIRED_PIN				2
#define BUTTONS_WIRED_HC165				3
#define BUTTONS_WIRED					BUTTONS_WIRED_HC165

#define BUTTONS_USART_OFF				1
#define BUTTONS_USART_ON				2
#define BUTTONS_USART					BUTTONS_USART_ON

#define BUTTONS_IRMP_OFF				1
#define BUTTONS_IRMP_ON					2
#define BUTTONS_IRMP					BUTTONS_IRMP_OFF

/* lcd */
#define LCD_CONTROLLER_OFF				1
#define LCD_CONTROLLER_SPLC780D1		2
#define LCD_CONTROLLER					LCD_CONTROLLER_OFF

/* temperature sensor */
#define TEMPERATURE_SENSOR_OFF			1
#define TEMPERATURE_SENSOR_TEST			2
#define TEMPERATURE_SENSOR_DS18B20		3
#define TEMPERATURE_SENSOR				TEMPERATURE_SENSOR_OFF

#endif

#endif /* MAIN_CONFIG_H_ */
