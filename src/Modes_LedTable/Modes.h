/*
 * Modes.h
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MODES_H_
#define MODES_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "AllOn/Mode_AllOn.h"
#include "RGBColorBlending/Mode_ColorBlending.h"
#include "Clock/Mode_Clock.h"
#include "Snake/Mode_Snake.h"


#define MODE__ALL_ON						1
#define MODE__BLENDING						(MODE__ALL_ON + 1)
#define MODE__BLENDING_SWEEP				(MODE__BLENDING + 1)
#define MODE__BLENDING_SWEEP_FAST			(MODE__BLENDING_SWEEP + 1)
#define MODE__BLENDING_CLOCK				(MODE__BLENDING_SWEEP_FAST + 1)
#define MODE__BLENDING_CLOCK_INVERTED		(MODE__BLENDING_CLOCK + 1)
#define MODE__CLOCK							(MODE__BLENDING_CLOCK_INVERTED + 1)
#define MODE__SNAKE							(MODE__CLOCK + 1)
#define MODE_NB								MODE__SNAKE


/* public functions */
void Modes__init (void);
void Modes__x10 (void);
void Modes__setMode (uint8_t mode);
uint8_t Modes__getMode (void);
void Modes__eepromStorage (void);
void Modes__savePreviousMode (void);
uint8_t Modes__getPreviousMode (void);


#endif /* MODES_H_ */
