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


#ifdef WS2801_NB

#define CHANNEL_NB		(WS2801_NB * 3)

/* public functions */

void WS2801__x10 (void);
void WS2801__setRGBForLED (RGB_Color_t color, uint16_t led);
void WS2801__setRGBForAllLEDs (RGB_Color_t color);
void WS2801__resetAllLEDs (void);

#endif
#endif /* WS2801_H_ */
