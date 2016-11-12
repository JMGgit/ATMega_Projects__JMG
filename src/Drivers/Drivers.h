/*
 * Drivers.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef DRIVERS_H_
#define DRIVERS_H_


#include "uC.h"
#include "DCF77/DCF77.h"
#include "DS1307/DS1307.h"
#include "HC165/HC165.h"
#include "WS2801/WS2801.h"
#include "WS2812/WS2812.h"
#include "APA102/APA102.h"
#include "SPLC780D1/SPLC780D1.h"
#include "DS18B20/DS18B20.h"
#include "IRMP/IRMP_Appl.h"
#include "Drivers_Config.h"


static inline void Drivers__init (void)
{
	Debug__setDriversState(DRIVERS_STATE_INIT_BEGIN);
#ifdef TEST1_LED_DDR
	setOutput(TEST1_LED_DDR, TEST1_LED_PIN);
#endif
#ifdef TEST2_LED_DDR
	setOutput(TEST2_LED_DDR, TEST2_LED_PIN);
#endif
#ifdef TEST3_LED_DDR
	setOutput(TEST3_LED_DDR, TEST3_LED_PIN);
#endif
	Debug__setDriversState(DRIVERS_STATE_INIT_UC);
	uC__init();
#if (BUTTONS_WIRED == BUTTONS_WIRED_HC165)
	Debug__setDriversState(DRIVERS_STATE_INIT_HC165);
	HC165__init();
#endif
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	Debug__setDriversState(DRIVERS_STATE_INIT_DS1307);
	DS1307__init();
#endif
#if (CLOCK_SYNC == CLOCK_SYNC_DCF77)
	Debug__setDriversState(DRIVERS_STATE_INIT_DCF77);
	DCF77__init();
#endif
#if (LCD_CONTROLLER == LCD_CONTROLLER_SPLC780D1)
	Debug__setDriversState(DRIVERS_STATE_INIT_SPLC780D1);
	SPLC780D1__init();
#endif
#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)
	Debug__setDriversState(DRIVERS_STATE_DS18B20);
	DS18B20__init();
#endif
#if (BUTTONS_IRMP != BUTTONS_IRMP_OFF)
	Debug__setDriversState(DRIVERS_STATE_INIT_IRMP);
	IRMP__init();
#endif
#if (LED_TYPE == LED_TYPE_WS2801)
	Debug__setDriversState(DRIVERS_STATE_INIT_WS2801);
	WS2801__init();
#endif
#if (LED_TYPE == LED_TYPE_WS2812)
	Debug__setDriversState(DRIVERS_STATE_INIT_WS2812);
	WS2812__init();
#endif
#if (LED_TYPE == LED_TYPE_APA102)
	Debug__setDriversState(DRIVERS_STATE_INIT_APA102);
	APA102__init();
#endif
	Debug__setDriversState(DRIVERS_STATE_INIT_END);
}


static inline void Drivers__begin_x10 (void)
{
	Debug__setDriversState(DRIVERS_STATE_X10_BEGIN1);
	Debug__setDriversState(DRIVERS_STATE_X10_UC);
	uC__begin_x10();
#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)
	Debug__setDriversState(DRIVERS_STATE_X10_DS18B0);
	DS18B20__x10();
#endif
#if (LED_TYPE == LED_TYPE_WS2801)
	Debug__setDriversState(DRIVERS_STATE_X10_WS2801);
	WS2801__x10();
#endif
#if (LED_TYPE == LED_TYPE_APA102)
	Debug__setDriversState(DRIVERS_STATE_X10_APA102);
	APA102__x10();
#endif
	Debug__setDriversState(DRIVERS_STATE_X10_END1);
}


static inline void Drivers__end_x10 (void)
{
	Debug__setDriversState(DRIVERS_STATE_X10_BEGIN2);
#if (LED_TYPE == LED_TYPE_WS2812)
	/* has to be called after modes to finish the SPI transfer */
	Debug__setDriversState(DRIVERS_STATE_X10_WS2812);
	WS2812__x10();
#endif
	Debug__setDriversState(DRIVERS_STATE_X10_UC_END);
	uC__end_x10(); /* has to be the last function called */
	Debug__setDriversState(DRIVERS_STATE_X10_END2);
}


#endif /* DRIVERS_H_ */
