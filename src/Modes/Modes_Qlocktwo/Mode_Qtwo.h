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


void Qtwo__init (void);
void Qtwo__main_x10 (void);
void Qtwo__seconds_x10 (void);
void Qtwo__timeSetup_x10 (void);
void Qtwo__modeTransition (void);
void Qtwo__setNextLang (void);


#endif /* MODE_QTWO_H_ */
