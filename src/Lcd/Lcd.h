/*
 * Lcd.h
 *
 *  Created on: 23.03.2014
 *      Author: Jean-Martin George
 */

#ifndef LCD_H_
#define LCD_H_


#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Drivers/Drivers.h"

void Lcd__init (void);
void Lcd__newLine (char* line);
void Lcd__writeLine (char* data, uint8_t line);
void Lcd__enableCursor (void);
void Lcd__disableCursor (void);
void Lcd__setCursor (uint8_t line, uint8_t col);
void Lcd__x10 (void);
void Lcd__requestRefreh (void);

#endif /* LCD_H_ */
