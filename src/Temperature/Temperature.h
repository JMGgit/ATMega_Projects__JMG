/*
 * Temperature.h
 *
 *  Created on: 25.05.2014
 *      Author: Jean-Martin George
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Drivers.h"

uint8_t Temperature__getCurrentRawValue (uint16_t *value);
void Temperature__getCurrentValues (uint8_t *negative,  uint8_t *t_int, uint8_t *t_frac);
void Temperature__getValuesFromRaw (uint16_t rawValue, uint8_t *negative,  uint8_t *t_int, uint8_t *t_frac);
uint16_t Temperature__getMinRawValue (uint16_t rawValue1, uint16_t rawValue2);
uint16_t Temperature__getMaxRawValue (uint16_t rawValue1, uint16_t rawValue2);
void Temperature__getCurrentValueString (char *buffer);
void Temperature__getValueString (uint8_t negative, uint8_t t_int, uint8_t t_frac, char *buffer);
void Temperature__getValueStringFromRaw (uint16_t rawValue, char *buffer);


#endif /* TEMPERATURE_H_ */
