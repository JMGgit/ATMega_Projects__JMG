/*
 * RGB_Tables.h
 *
 *  Created on: 04.06.2013
 *      Author: Jean-Martin George
 */

#ifndef RGB_TABLES_H_
#define RGB_TABLES_H_


#include "Main_Types.h"


#define NUMBER_OF_BLENDING_COLORS				16
#define NUMBER_OF_BLENDING_COLORS_ARRAY_SIZE	(NUMBER_OF_BLENDING_COLORS * 3)

extern const uint8_t RGB_BlendingColors[NUMBER_OF_BLENDING_COLORS_ARRAY_SIZE];

extern const uint8_t PWM_Table_32[32];
extern const uint8_t PWM_Table_64[64];
extern const uint8_t PWM_Table_256[256];

#endif /* RGB_TABLES_H_ */
