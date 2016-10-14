/*
 * Debug_Def.h
 *
 *  Created on: 12.10.2016
 *      Author: Jean-Martin George
 */

#ifndef DEBUG_DEF_H_
#define DEBUG_DEF_H_

#include "Main_Config.h"

#if (DEBUG_MODE == DEBUG_MODE_ON)
extern uint8_t driversState;
extern uint8_t mainState;
extern uint8_t modeState;
extern uint8_t whileState;
#endif


static inline void Debug__setDriversState (uint8_t state)
{
#if (DEBUG_MODE == DEBUG_MODE_ON)
	driversState = state;
#endif
}


static inline void Debug__setMainState (uint8_t state)
{
#if (DEBUG_MODE == DEBUG_MODE_ON)
	mainState = state;
#endif
}


static inline void Debug__setModeState (uint8_t state)
{
#if (DEBUG_MODE == DEBUG_MODE_ON)
	modeState = state;
#endif
}


static inline void Debug__setWhileState (uint8_t state)
{
#if (DEBUG_MODE == DEBUG_MODE_ON)
	whileState = state;
#endif
}


#define DRIVERS_STATE_INIT_BEGIN		0x01
#define DRIVERS_STATE_INIT_UC			0x02
#define DRIVERS_STATE_INIT_HC165		0x03
#define DRIVERS_STATE_INIT_DS1307		0x04
#define DRIVERS_STATE_INIT_DCF77		0x05
#define DRIVERS_STATE_INIT_SPLC780D1	0x06
#define DRIVERS_STATE_INIT_DSB18B20		0x07
#define DRIVERS_STATE_INIT_IRMP			0x08
#define DRIVERS_STATE_INIT_WS2801		0x09
#define DRIVERS_STATE_INIT_WS2812		0x0A
#define DRIVERS_STATE_INIT_END			0x0B

#define DRIVERS_STATE_X10_BEGIN1		0x11
#define DRIVERS_STATE_X10_UC			0x12
#define DRIVERS_STATE_X10_DS18B0		0x13
#define DRIVERS_STATE_X10_WS2801		0x14
#define DRIVERS_STATE_X10_END1			0x15

#define DRIVERS_STATE_X10_BEGIN2		0x20
#define DRIVERS_STATE_X10_WS2812		0x21
#define DRIVERS_STATE_X10_UC_END		0x22
#define DRIVERS_STATE_X10_END2			0x23

#define MAIN_STATE_INIT_BEGIN			0x01
#define MAIN_STATE_INIT_FAILUREMEMORY	0x02
#define MAIN_STATE_INIT_DRIVERS			0x03
#define MAIN_STATE_INIT_BUTTONS			0x04
#define MAIN_STATE_INIT_CLOCK			0x05
#define MAIN_STATE_INIT_LCD				0x06
#define MAIN_STATE_INIT_LEDMATRIX		0x07
#define MAIN_STATE_INIT_MODES			0x08
#define MAIN_STATE_X10_LEDLOOP			0x09

#define MAIN_STATE_X10_BEGIN			0x10
#define MAIN_STATE_X10_DEBUG			0x11
#define MAIN_STATE_X10_DRIVERS_UC_TASK	0x12
#define MAIN_STATE_X10_DRIVERS_BEGIN	0x13
#define MAIN_STATE_X10_LCD				0x14
#define MAIN_STATE_X10_CLOCK			0x15
#define MAIN_STATE_X10_BUTTONS			0x16
#define MAIN_STATE_X10_MODES			0x17
#define MAIN_STATE_X10_DRIVERS_END		0x18
#define MAIN_STATE_X10_END				0x19

#define WHILE_STATE_BUTTONS_BEFORE		0x01
#define WHILE_STATE_BUTTONS_AFTER		0x02
#define WHILE_STATE_ADC1_BEFORE			0x03
#define WHILE_STATE_ADC1_AFTER			0x04
#define WHILE_STATE_ADC2_BEFORE			0x05
#define WHILE_STATE_ADC2_AFTER			0x06
#define WHILE_STATE_SPI_BEFORE			0x07
#define WHILE_STATE_SPI_AFTER			0x08
#define WHILE_STATE_TWIS_BEFORE			0x09
#define WHILE_STATE_TWIS_AFTER			0x10
#define WHILE_STATE_UC_BEFORE			0x11
#define WHILE_STATE_UC_AFTER			0x12
#define WHILE_STATE_USART_BEFORE		0x13
#define WHILE_STATE_USART_AFTER			0x14
#define WHILE_STATE_DS13071_BEFORE		0x15
#define WHILE_STATE_DS13071_AFTER		0x16
#define WHILE_STATE_DS13072_BEFORE		0x17
#define WHILE_STATE_DS13072_AFTER		0x18
#define WHILE_STATE_DS13073_BEFORE		0x19
#define WHILE_STATE_DS13073_AFTER		0x20
#define WHILE_STATE_DS13074_BEFORE		0x21
#define WHILE_STATE_DS13074_AFTER		0x22
#define WHILE_STATE_DS13075_BEFORE		0x23
#define WHILE_STATE_DS13075_AFTER		0x24
#define WHILE_STATE_DS13076_BEFORE		0x25
#define WHILE_STATE_DS13076_AFTER		0x26
#define WHILE_STATE_DS13077_BEFORE		0x27
#define WHILE_STATE_DS13077_AFTER		0x28

#endif /* DEBUG_DEF_H_ */
