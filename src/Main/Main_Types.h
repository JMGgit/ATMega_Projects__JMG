/*
 * Main_Types.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef MAIN_TYPES_H_
#define MAIN_TYPES_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>


#define FALSE	0x00
#define TRUE	0x01

#define E_OK		0
#define E_NOT_OK	1

#define INLINE static inline

#define MIN(a, b) (((a) < (b)) ? (a) : (b) )
#define MAX(a, b) (((a) > (b)) ? (a) : (b) )

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} RGB_Color_t;


/* ToDO: move functions */
INLINE RGB_Color_t getRGBColorFromComponents (uint8_t red, uint8_t green, uint8_t blue)
{
	return ((RGB_Color_t){red, green, blue});
}


#endif /* MAIN_TYPES_H_ */