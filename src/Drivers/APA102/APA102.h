/*
 * APA102.h
 *
 *  Created on: 27.10.2016
 *      Author: Jean-Martin George
 */

#ifndef APA102_H_
#define APA102_H_

#include <avr/io.h>
#include <string.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "uC.h"


#if (LED_TYPE == LED_TYPE_APA102)

void APA102__init (void);
void APA102__x10 (void);
void APA102__setRGBForLED (RGB_Color_t color, uint16_t led);
void APA102__setRGBForAllLEDs (RGB_Color_t color);
void APA102__resetAllLEDs (void);
void APA102__enableUpdate (uint8_t enable);
void APA102__disableUpdate (uint8_t enable);
void APA102__setGlobalBrightness (uint8_t brightness);
#if (RGB_LED_ORDER == RGB_LED_ORDER__CONFIGURABLE)
void APA102__toggleRGBLedOrder (void);
#endif

#define APA102_GLOBAL_BRIGHNESS__MAX	31
#define APA102_GLOBAL_BRIGHNESS__MIN	1

#endif

#endif /* APA102_H_ */
