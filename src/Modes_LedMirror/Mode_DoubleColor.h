/*
 * Mode_DoubleColor.h
 *
 *  Created on: 17.10.2015
 *      Author: Jean-Martin George
 */

#ifndef SRC_MODES_LEDMIRROR_MODE_DOUBLECOLOR_H_
#define SRC_MODES_LEDMIRROR_MODE_DOUBLECOLOR_H_


#include "Main_Types.h"
#include "Main_Config.h"
#include "LEDMatrix.h"
#include "RGB_Tables.h"


#define UPDATE_TIME		30
#define SNAKE_SIZE		7

void DoubleColor__UpdateMatrix_x10 (void);

#endif /* SRC_MODES_LEDMIRROR_MODE_DOUBLECOLOR_H_ */
