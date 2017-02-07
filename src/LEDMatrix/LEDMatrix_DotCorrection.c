/*
 * LEDMatrix_DotCorrection.c
 *
 *  Created on: 30.11.2013
 *      Author: Jean-Martin George
 */


#include "LEDMatrix_DotCorrection.h"


#ifdef LED_MATRIX_SIZE_LIN

void LEDMatrix__applyDotCorrection (RGB_Color_t* color, uint8_t line, uint8_t column)
{
#if (LED_COLORS == LED_COLORS_PWM)
	color->red = PWM_Table_256[color->red];
	color->green = PWM_Table_256[color->green];
	color->blue = PWM_Table_256[color->blue];
#endif

	if ((color->red == color->blue) && (color->blue == color->green))
	{
		/* correction only for white */
		color->red = (color->red * WHITE_COLOR_PERCENT_RED) / 100;
		color->green = (color->green * WHITE_COLOR_PERCENT_GREEN) / 100;
		color->blue = (color->blue * WHITE_COLOR_PERCENT_BLUE) / 100;
	}
}

#endif
