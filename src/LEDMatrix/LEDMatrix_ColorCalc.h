/*
 * LEDMatrix_ColorCalc.h
 *
 *  Created on: 03.11.2015
 *      Author: Jean-Martin George
 */

#ifndef LEDMATRIX_COLORCALC_H_
#define LEDMATRIX_COLORCALC_H_


#include "Main_Types.h"
#include "Main_Config.h"
#include "Drivers.h"
#include "Buttons.h"

RGB_Color_t LEDMatrix__getColorFromInputs (void);


static inline  RGB_Color_t LEDMatrix__getRGBColorFromComponents (uint8_t red, uint8_t green, uint8_t blue)
{
	return ((RGB_Color_t){red, green, blue});
}


static inline void LEDMatrix__setRGBColorFromComponents (RGB_Color_t *color, uint8_t red, uint8_t green, uint8_t blue)
{
	color->red = red;
	color->green = green;
	color->blue = blue;
}

#endif /* LEDMATRIX_COLORCALC_H_ */
