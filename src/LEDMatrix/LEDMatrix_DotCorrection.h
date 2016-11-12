/*
 * LEDMatrix_DotCorrection.h
 *
 *  Created on: 30.11.2013
 *      Author: Jean-Martin George
 */

#ifndef LEDMATRIX_DOTCORRECTION_H_
#define LEDMATRIX_DOTCORRECTION_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "RGB_Tables.h"

void LEDMatrix__applyDotCorrection (RGB_Color_t* color, uint8_t line, uint8_t column);

#endif /* LEDMATRIX_DOTCORRECTION_H_ */
