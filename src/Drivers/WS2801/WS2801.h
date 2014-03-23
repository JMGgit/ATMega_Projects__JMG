/*
 * WS2801.h
 *
 *  Created on: 21.02.2013
 *      Author: Jean-Martin George
 */

#ifndef WS2801_H_
#define WS2801_H_

#include <avr/io.h>
#include "../../Main/Main_Types.h"
#include "../Drivers_Config.h"
#include "../AtMega/AtMega.h"


#ifdef WS2801_NB

#define CHANNEL_NB		(WS2801_NB * 3)

/* public functions */
void WS2801__x10 (void);

/* private functions */
void WS2801__updateAll(void);
void WS2801__setGSForChannel (uint8_t gsData, uint16_t channel);
void WS2801__setRGBForLED (RGB_Color_t color, uint16_t led);

#endif
#endif /* WS2801_H_ */
