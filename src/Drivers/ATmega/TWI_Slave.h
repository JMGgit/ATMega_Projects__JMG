/*
 * TWI_Slave.h
 *
 *  Created on: 16.03.2016
 *      Author: Jean-Martin George
 */

#ifndef TWI_SLAVE_H_
#define TWI_SLAVE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Ports.h"
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"

#if (TWI_MODE == TWI_MODE_SLAVE)

void TWI__slaveInit (uint8_t *txData, uint8_t txDataLength);
void TWI__slaveUpdateTxData (void);

#define TWI_SLAVE_ADDRESS		0x10
#define TWI_DATA_LENGTH_MAX		2

#endif

#endif /* TWI_H_ */
