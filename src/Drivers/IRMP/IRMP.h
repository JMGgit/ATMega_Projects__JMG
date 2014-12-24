/*
 * IRMP.h
 *
 *  Created on: 23.12.2014
 *      Author: JMG
 */

#ifndef SRC_DRIVERS_IRMP_IRMP_H_
#define SRC_DRIVERS_IRMP_IRMP_H_

#include <avr/io.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "AtMega/AtMega.h"
#include "irmpLib/irmp.h"

#if (IRMP == IRMP_ON)

void IRMP__init (void);
uint8_t IRMP__readData (uint8_t *data, uint8_t dataLength);

#endif

#endif /* SRC_DRIVERS_IRMP_IRMP_H_ */
