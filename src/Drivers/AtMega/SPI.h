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
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "Ports.h"


#define SPI_DDR			DDRB
#define SPI_PORT		PORTB
#define SPI_PIN_SS		PB4
#define SPI_PIN_MOSI	PB5
#define SPI_PIN_MISO	PB6
#define SPI_PIN_CLK		PB7


void SPI__masterInit (void);
void SPI__transmitDataBytes (uint8_t *data, uint8_t dataLength);


#endif /* SPI_H_ */
