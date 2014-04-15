/*
 * SPLC780D1.h
 *
 *  Created on: 22.03.2014
 *      Author: Jean-Martin George
 */

#ifndef SPLC780D1_H_
#define SPLC780D1_H_


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "../../Main/Main_Types.h"
#include "../../Main/Main_Config.h"
#include "../Drivers_Config.h"
#include "../AtMega/AtMega.h"


void SPLC780D1__init (void);
void SPLC780D1__sendCommand (uint8_t data);
void SPLC780D1__sendData (uint8_t data);
void SPLC780D1__sendTable (uint8_t *data);
void SPLC780D1__x10 (void);


#endif /* SPLC780D1_H_ */
