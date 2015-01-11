/*
 * HC165.h
 *
 *  Created on: 02.03.2013
 *      Author: Jean-Martin George
 */

#ifndef HC165_H_
#define HC165_H_

#include <avr/io.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "AtMega/AtMega.h"


#if (BUTTONS_WIRED == BUTTONS_WIRED_HC165)

/* public functions */
void HC165__init (void);
void HC165__readByte (uint8_t *buffer);

#endif

#endif /* HC165_H_ */
