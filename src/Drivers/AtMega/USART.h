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
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "Ports.h"


void USART__init (void);
#if (USART_DATA_LENGTH_READ_MAX != 0)
uint8_t USART__readData (uint8_t *data, uint8_t dataLength, uint8_t requester);
#endif
void USART__transmitData (uint8_t *data, uint8_t dataLength);


#endif /* USART_H_ */
