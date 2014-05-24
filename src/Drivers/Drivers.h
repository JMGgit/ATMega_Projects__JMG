/*
 * Drivers.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef DRIVERS_H_
#define DRIVERS_H_


#include "AtMega/uC.h"
#include "DCF77/DCF77.h"
#include "DS1307/DS1307.h"
#include "HC165/HC165.h"
#include "WS2801/WS2801.h"
#include "SPLC780D1/SPLC780D1.h"
#include "DS18B20/DS18B20.h"
#include "Drivers_Config.h"

INLINE void Drivers__init (void)
{
	uC__init();
#if (BUTTONS_MULTIPLEX == BUTTONS_MULTIPLEX_HC165)
	HC165__init();
#endif
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	DS1307__init();
#endif
#if (CLOCK_SYNC == CLOCK_SYNC_DCF77)
	DCF77__init();
#endif
	USART__init();
	ADC__init();
#if (LCD_CONTROLLER == LCD_CONTROLLER_SPLC780D1)
	SPLC780D1__init();
#endif
#if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR_DS18B20)
	DS18B20__init();
#endif
}


INLINE void Drivers__x10 (void)
{
#ifdef WS2801_NB
	WS2801__x10();
#endif
}

#endif /* DRIVERS_H_ */
