/*
 * Mode_ColorBlending.h
 *
 *  Created on: 04.06.2013
 *      Author: Jean-Martin George
 */

#ifndef MODE_COLORBLENDING_H_
#define MODE_COLORBLENDING_H_


#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../LEDMatrix/LEDMatrix.h"
#include "../ColorTables/RGB_Tables.h"


void ColorBlending__updateMatrix (void);
RGB_Color_t ColorBlending__getCurrentColor (void);
RGB_Color_t ColorBlending__getCurrentColorCol (uint8_t col);

#endif /* MODE_COLORBLENDING_H_ */
