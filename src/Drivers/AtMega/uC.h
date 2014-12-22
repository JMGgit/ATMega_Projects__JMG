/*
 * uC.h
 *
 *  Created on: 28.03.2013
 *      Author: Jean-Martin George
 */

#ifndef UC_H_
#define UC_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "Ports.h"
#include "uC.h"
#include "SPI.h"
#include "TWI.h"
#include "USART.h"


void uC__init (void);
uint8_t uC__isTaskTrigger_x10 (void);


#endif /* UC_H_ */
