/*
 * USART.h
 *
 *  Created on: 28.03.2013
 *      Author: Jean-Martin George
 */

#ifndef USART_H_
#define USART_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "../../Main/Main_Types.h"
#include "../Drivers_Config.h"
#include "Ports.h"


void USART__init (void);
uint8_t USART__readDataBytes (uint8_t *data, uint8_t dataLength, uint8_t requester);
void USART__transmitDataBytes (uint8_t *data, uint8_t dataLength);


#endif /* USART_H_ */
