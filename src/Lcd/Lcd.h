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
void Lcd__writeLine (uint8_t *data, uint8_t line);
void Lcd__x10 (void);


#endif /* LCD_H_ */
