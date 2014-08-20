/*
 * Mode_AllOn.c
 *
 *  Created on: 02.03.2013
 *      Author: Jean-Martin George
 */

#include "Mode_AllOn.h"


void AllOn__updateMatrix (void)
{
	uint16_t linIt, colIt;

	for (linIt = 1; linIt <= LED_MATRIX_SIZE_LIN; linIt++)
	{
		for (colIt = 1; colIt <= LED_MATRIX_SIZE_COL; colIt++)
		{
			LEDMatrix__setRGBColor(linIt, colIt, Modes_currentColor);
		}
	}
}
