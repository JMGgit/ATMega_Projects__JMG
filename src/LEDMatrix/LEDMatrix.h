/*
 * LEDMatrix.h
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#ifndef LEDMATRIX_H_
#define LEDMATRIX_H_


#include "Main_Types.h"
#include "Main_Config.h"
#include "Drivers.h"
#include "LEDMatrix_DotCorrection.h"
#include "LEDMatrix_ColorCalc.h"


void LEDMatrix__setRGBColor (uint8_t line, uint8_t column, RGB_Color_t color);
void LEDMatrix__setRGBColorForMatrix (RGB_Color_t color);
void LEDMatrix__clearMatrix (void);
void LEDMatrix__toggleledOrder (void);


#endif /* LEDMATRIX_H_ */
