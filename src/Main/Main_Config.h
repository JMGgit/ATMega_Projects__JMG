/*
 * Main_Config.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_


/* size of the LED matrix */
#define LED_MATRIX_SIZE_LIN				10
#define LED_MATRIX_SIZE_COL				11

/* dot correction for LEDs (white color) */
#define WHITE_COLOR_PERCENT_RED			100
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
#define CLOCK_SYNC						CLOCK_SYNC_OFF

/* buttons */
#define BUTTONS_MULTIPLEX_OFF			1
#define BUTTONS_MULTIPLEX_HC165			2
#define BUTTONS_MULTIPLEX				BUTTONS_MULTIPLEX_OFF

#endif /* MAIN_CONFIG_H_ */
