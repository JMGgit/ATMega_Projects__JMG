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

/* clock sync */
#define CLOCK_SYNC_OFF					1
#define CLOCK_SYNC_DCF77				2
#define CLOCK_SYNC_USART				3
#define CLOCK_SYNC						CLOCK_SYNC_OFF

/* buttons */
#define BUTTONS_MULTIPLEX_OFF			1
#define BUTTONS_MULTIPLEX_HC165			2
#define BUTTONS_MULTIPLEX				BUTTONS_MULTIPLEX_OFF

/* measurement */
#define MAX_MEASUREMENT_POINTS			2000

#endif /* MAIN_CONFIG_H_ */
