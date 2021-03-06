/*
 * TWI_Master.h
 *
 *  Created on: 27.03.2013
 *      Author: Jean-Martin George
 */

#ifndef TWI_MASTER_H_
#define TWI_MASTER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Ports.h"
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"


#if (TWI_MODE == TWI_MODE_MASTER)

#define TWI_MASTER_READ_BIT		1
#define TWI_MASTER_WRITE_BIT	0


void TWI__masterInit (void);
uint8_t TWI__masterReadData (uint8_t *data, uint8_t dataLength, uint8_t slaveAddress);
uint8_t TWI__masterTransmitData (uint8_t *data, uint8_t dataLength, uint8_t slaveAddress);

#endif

#endif /* TWI_H_ */
