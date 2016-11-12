/*
 * SPI.h
 *
 *  Created on: 27.03.2013
 *      Author: Jean-Martin George
 */

#ifndef SPI_H_
#define SPI_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include "Ports.h"
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"


#if (SPI_SPEED != SPI_DISABLED)

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega1284P__)
#define SPI_DDR			DDRB
#define SPI_PORT		PORTB
#define SPI_PIN_SS		PB4
#define SPI_PIN_MOSI	PB5
#define SPI_PIN_MISO	PB6
#define SPI_PIN_CLK		PB7
#endif

#ifdef __AVR_ATmega328P__
#define SPI_DDR			DDRB
#define SPI_PORT		PORTB
#define SPI_PIN_SS		PB2
#define SPI_PIN_MOSI	PB3
#define SPI_PIN_MISO	PB4
#define SPI_PIN_CLK		PB5
#endif

void SPI__masterInit (void);
void SPI__transmitData (uint8_t *data, uint16_t dataLength);
void SPI__slaveSelect (volatile uint8_t *slaveDdr, volatile uint8_t *slavePort, uint8_t slavePin, uint8_t enableSlave);

#endif

#endif /* SPI_H_ */
