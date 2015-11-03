/*
 * Mode_AllOn.c
 *
 *  Created on: 02.03.2013
 *      Author: Jean-Martin George
 */

#include "Mode_AllOn.h"


static RGB_Color_t currentColor = {255, 255, 255};

void AllOn__updateMatrix (void)
{
	currentColor = LEDMatrix__getColorFromInputs();
	LEDMatrix__setRGBColorForMatrix(currentColor);
}
