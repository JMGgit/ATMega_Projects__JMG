/*
 * Drivers_Config.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef DRIVERS_CONFIG_H_
#define DRIVERS_CONFIG_H_


#include "Qlocktwo_Drivers_Config.h"
#include "LedTable_Drivers_Config.h"
#include "LedFrame_Drivers_Config.h"
#include "LedMirror_Drivers_Config.h"
#include "TemperatureLogger_Drivers_Config.h"
#include "IRMP_Drivers_Config.h"


#if (LED_TYPE == LED_TYPE_WS2801)
#if (LEDS_NB < 100)
#define SPI_SPEED SPI_CLK_DIV_8
#else
#define SPI_SPEED SPI_CLK_DIV_16
#endif
#endif

#if (LED_TYPE == LED_TYPE_WS2812)
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIGIDOT_SPI)
#define SPI_SPEED				SPI_CLK_DIV_2
#endif
#endif

/* TWI MODE */
#define TWI_MODE_DISABLED		1
#define TWI_MODE_MASTER			2
#define TWI_MODE_SLAVE			3

#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
#define TWI_MODE 				TWI_MODE_MASTER
#elif (BUTTONS_IRMP == BUTTONS_IRMP_SEND_TO_TWI)
#define TWI_MODE 				TWI_MODE_SLAVE
#else /* no TWI needed */
#define TWI_MODE 				TWI_MODE_DISABLED
#endif

#endif /* DRIVERS_CONFIG_H_ */
