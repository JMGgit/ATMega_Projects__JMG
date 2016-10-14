/*
 * Mode_Qlocktwo.h
 *
 *  Created on: 19.09.2013
 *      Author: JMG
 */

#ifndef MODE_QTWO_H_
#define MODE_QTWO_H_


#include "Main_Types.h"
#include "Main_Config.h"
#include "LEDMatrix.h"
#include "Clock.h"
#include "Buttons.h"


typedef enum
{
	QTWO_MODE_NORMAL = 0,
	QTWO_MODE_SECONDS,
	QTWO_MODE_SETUP
} QtwoMode_N;


void Qtwo__init (void);
void Qtwo__main_x10 (QtwoMode_N qtwoMode);
void Qtwo__modeTransition (void);
void Qtwo__setNextLang (void);
uint8_t Qtwo__getCurrentBrightness (void);
RGB_Color_t Qtwo__getCurrentColor (void);


#endif /* MODE_QTWO_H_ */
