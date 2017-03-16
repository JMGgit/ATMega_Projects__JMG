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
#include "Ports.h"
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"


#if (USART_DATA_LENGTH_READ_MAX != 0)

void USART__init (void);
uint8_t USART__readData (uint8_t *data, uint8_t dataLength, uint8_t requester);
void USART__sendData (uint8_t *data, uint8_t dataLength);
void USART__sendString (char *string);

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega1284P__)
uint8_t USART1__readData (uint8_t *data, uint8_t dataLength, uint8_t requester);
void USART1__sendData (uint8_t *data, uint8_t dataLength);
void USART1__sendString (char *string);
#endif

#endif

#endif /* USART_H_ */
