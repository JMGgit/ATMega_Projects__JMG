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
#include "SPLC780D1/SPLC780D1.h"
#include "DS18B20/DS18B20.h"
#include "IRMP/IRMP_Appl.h"
#include "Drivers_Config.h"


static inline void Drivers__init (void)
{
#ifdef TEST1_LED_DDR
	setOutput(TEST1_LED_DDR, TEST1_LED_PIN);
#endif
#ifdef TEST2_LED_DDR
	setOutput(TEST2_LED_DDR, TEST2_LED_PIN);
#endif
#ifdef TEST3_LED_DDR
	setOutput(TEST3_LED_DDR, TEST3_LED_PIN);
#endif
	uC__init();
#if (BUTTONS_WIRED == BUTTONS_WIRED_HC165)
	HC165__init();
#endif
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	DS1307__init();
#endif
#if (CLOCK_SYNC == CLOCK_SYNC_DCF77)
	DCF77__init();
#endif
#if (LCD_CONTROLLER == LCD_CONTROLLER_SPLC780D1)
	SPLC780D1__init();
#endif
#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)
	DS18B20__init();
#endif
#if (BUTTONS_IRMP != BUTTONS_IRMP_OFF)
	IRMP__init();
#endif
#ifdef WS2812_NB
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIGIDOT_SPI)
	WS2812_DigiDotBooster__init();
#endif
#endif
}


static inline void Drivers__x10 (void)
{
#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)
	DS18B20__x10();
#endif
#ifdef WS2801_NB
	WS2801__x10();
#endif
}


static inline void Drivers__end__x10 (void)
{
#ifdef WS2812_NB
	/* has to be called after modes to finish the SPI transfer */
	WS2812__x10();
#endif
}

#endif /* DRIVERS_H_ */
