/*
 * WS2801.h
 *
 *  Created on: 21.02.2013
 *      Author: Jean-Martin George
 */

#ifndef WS2801_H_
#define WS2801_H_

#include <avr/io.h>
#include <string.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "uC.h"


#if (LED_TYPE == LED_TYPE_WS2801)

#define CHANNEL_NB		(LEDS_NB * 3)

/* public functions */

void WS2801__init (void);
void WS2801__x10 (void);
void WS2801__setRGBForLED (RGB_Color_t color, uint16_t led);
void WS2801__setRGBForAllLEDs (RGB_Color_t color);
void WS2801__resetAllLEDs (void);
void WS2801__enableUpdate (uint8_t enable);
void WS2801__disableUpdate (uint8_t enable);
#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
void WS2801__toggleRGBLedOrder (void);
#endif

#endif
#endif /* WS2801_H_ */
