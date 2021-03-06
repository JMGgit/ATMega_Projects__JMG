/*
 * WS2812_DigiDotBooster.h
 *
 *  Created on: 28.03.2016
 *      Author: Jean-Martin George
 */

#ifndef WS2812_DIGIDOTBOOSTER_H_
#define WS2812_DIGIDOTBOOSTER_H_


#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "uC.h"

#if (LED_TYPE == LED_TYPE_WS2812)
#if (WS2812_CONNECTION_TYPE == WS2812_CONNECTION_TYPE_DIGIDOT_SPI)

void WS2812_DigiDotBooster__init (void);
void WS2812_DigiDotBooster__setRGBForLED (RGB_Color_t color, uint8_t led);
void WS2812_DigiDotBooster__setRGBForAllLEDs (RGB_Color_t color);
void WS2812_DigiDotBooster__x10 (void);

#endif
#endif

#endif /* WS2812_DIGIDOTBOOSTER_H_ */
