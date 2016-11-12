/*
 * Drivers_Config_Def.h
 *
 *  Created on: 10.04.2016
 *      Author: Jean-Martin George
 */

#ifndef DRIVERS_CONFIG_DEF_H_
#define DRIVERS_CONFIG_DEF_H_


#include "Main_Config_Def.h"

/* type of LEDs */
#define LED_TYPE_WS2801						1
#define LED_TYPE_WS2812						2
#define LED_TYPE_APA102						3

#define RGB_LED_ORDER__RED_GREEN_BLUE		1
#define RGB_LED_ORDER__BLUE_GREEN_RED		2
#define RGB_LED_ORDER__CONFIGURABLE			3

#define LED_ORDER__LEFT_2_RIGHT				1
#define LED_ORDER__STRAIGHT_FORWARD			2
#define LED_ORDER__CONFIGURABLE				3

/* WS2812 */
#define WS2812_CONNECTION_TYPE_DIGIDOT_SPI	1
#define WS2812_CONNECTION_TYPE_DIRECT		2

/* SPI SPEED */
#define SPI_CLK_DIV_16						1
#define SPI_CLK_DIV_8						2
#define SPI_CLK_DIV_2						3
#define SPI_DISABLED						255

/* TWI MODE */
#define TWI_MODE_DISABLED	    			1
#define TWI_MODE_MASTER	    				2
#define TWI_MODE_SLAVE			    		3

/* IRMP */
#define IRMP_REMOTE_ADDRESS					0xF708
#define IRMP_BUTTON_OFF						0x001B
#define IRMP_BUTTON_MODE					0x0004
#define IRMP_BUTTON_FUNC1					0x001F
#define IRMP_BUTTON_FUNC2					0x001E
#define IRMP_BUTTON_FUNC3					0x001A
#define IRMP_BUTTON_UP						0x0005
#define IRMP_BUTTON_DOWN					0x0000
#define IRMP_BUTTON_LEFT					0x0008
#define IRMP_BUTTON_RIGHT					0x0001

#endif /* DRIVERS_DRIVERS_CONFIG_DEF_H_ */
