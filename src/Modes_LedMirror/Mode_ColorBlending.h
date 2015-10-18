/*
 * Mode_ColorBlending.h
 *
 *  Created on: 04.06.2013
 *      Author: Jean-Martin George
 */

#ifndef MODE_COLORBLENDING_H_
#define MODE_COLORBLENDING_H_


#include "Main_Types.h"
#include "Main_Config.h"
#include "LEDMatrix.h"
#include "RGB_Tables.h"


void ColorBlending__updateMatrix (uint8_t blendingMode);
RGB_Color_t ColorBlending__getCurrentColor (void);
RGB_Color_t ColorBlending__getCurrentColorCol (uint8_t col);

#define BLENDING_MODE_SLOW				0
#define BLENDING_MODE_SLOW_2_COLORS		1
#define BLENDING_MODE_FAST				2
#define BLENDING_MODE_FAST_2_COLORS		3
#define BLENDING_MODE_SWEEP				4
#define BLENDING_MODE_SWEEP_FAST		5

#endif /* MODE_COLORBLENDING_H_ */
