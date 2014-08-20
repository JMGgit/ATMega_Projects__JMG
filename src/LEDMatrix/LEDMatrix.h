/*
 * LEDMatrix.h
 *
 *  Created on: 20.01.2013
 *      Author: Jean-Martin George
 */

#ifndef LEDMATRIX_H_
#define LEDMATRIX_H_


#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Drivers/WS2801/WS2801.h"


void LEDMatrix__setRGBColor (uint8_t line, uint8_t column, RGB_Color_t color);
void LEDMatrix__clearMatrix (void);

extern RGB_Color_t Modes_currentColor;
#endif /* LEDMATRIX_H_ */