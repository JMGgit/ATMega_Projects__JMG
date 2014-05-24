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


#if (LCD_CONTROLLER == LCD_CONTROLLER_SPLC780D1)

void SPLC780D1__init (void);
void SPLC780D1__sendCommand (char data);
void SPLC780D1__sendData (char data);
void SPLC780D1__sendLine (char data, uint8_t line);
void SPLC780D1__sendTable (char *data);
void SPLC780D1__enableCursor (void);
void SPLC780D1__disableCursor (void);
void SPLC780D1__setCursor (uint8_t position);
void SPLC780D1__setCursorToLineCol (uint8_t line, uint8_t col);

#endif

#endif /* SPLC780D1_H_ */
