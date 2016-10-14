/*
 * Main.h
 *
 *  Created on: 15.03.2013
 *      Author: Jean-Martin George
 */

#ifndef MAIN_H_
#define MAIN_H_


#include "Main_Types.h"
#include "Main_Config.h"
#include "Drivers.h"
#include "Buttons.h"
#if (DEBUG_MODE == DEBUG_MODE_ON)
#include "FailureMemory.h"
#endif
#if (PROJECT != PROJECT__IRMP)
#include "Clock.h"
#include "Modes.h"
#include "Lcd.h"
#include "LEDMatrix.h"
#endif

#endif /* MAIN_H_ */
