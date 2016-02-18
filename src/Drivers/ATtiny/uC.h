/*
 * uC.h
 *
 *  Created on: 05.02.2016
 *      Author: Jean-Martin George
 */

#ifndef UC_H_
#define UC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Ports.h"
#include "TWI.h"
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"


#ifndef F_CPU
#error F_CPU not defined!
#endif

void uC__init (void);

#endif /* UC_H_ */
