/*
 * TWI.h
 *
 *  Created on: 27.03.2013
 *      Author: Jean-Martin George
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "Ports.h"


#define TWI_READ_BIT	1
#define TWI_WRITE_BIT	0


void TWI__masterInit (void);
uint8_t TWI__readData (uint8_t *data, uint8_t dataLength, uint8_t slaveAddress);
uint8_t TWI__transmitData (uint8_t *data, uint8_t dataLength, uint8_t slaveAddress);
