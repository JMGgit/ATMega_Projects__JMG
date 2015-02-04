/*
 * Mode_Startup.h
 *
 *  Created on: 25.01.2014
 *      Author: JMG
 */

#ifndef MODE_STARTUP_H_
#define MODE_STARTUP_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "LEDMatrix.h"


typedef enum
{
	TEST_RED = 0,
	TEST_GREEN,
	TEST_BLUE,
	TEST_WHITE,
	TEST_DONE
} testLedStates_N;


void Mode__Startup_x10 (void);


#endif /* MODE_STARTUP_H_ */
