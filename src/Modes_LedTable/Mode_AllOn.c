/*
 * Mode_AllOn.c
 *
 *  Created on: 02.03.2013
 *      Author: Jean-Martin George
 */

#include "Mode_AllOn.h"


void AllOn__updateMatrix (void)
{
	LEDMatrix__setRGBColorForMatrix(Modes_currentColor);
}
