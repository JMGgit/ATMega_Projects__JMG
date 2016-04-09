/*
 * WS2812.h
 *
 *  Created on: 16.01.2016
 *      Author: Jean-Martin George
 */

#ifndef WS2812_H_
#define WS2812_H_


#include <avr/io.h>
#include <string.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "WS2812lib/light_ws2812.h"
#include "WS2812_DigidotBooster/WS2812_DigiDotBooster.h"
#include "uC.h"

#if (LED_TYPE == LED_TYPE_WS2812)

void WS2812__init (void);
void WS2812__x10 (void);
void WS2812__setRGBForLED (RGB_Color_t color, uint16_t led);
void WS2812__setRGBForAllLEDs (RGB_Color_t color);
void WS2812__resetAllLEDs (void);
#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
void WS2812__toggleledOrder (void);
#endif

#endif

#endif /* SRC_DRIVERS_WS2812_WS2812_H_ */
