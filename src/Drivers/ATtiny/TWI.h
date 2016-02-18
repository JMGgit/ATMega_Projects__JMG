/*
 * TWI.h
 *
 *  Created on: 05.02.2016
 *      Author: Jean-Martin George
 */

#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Ports.h"
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "TWI_lib/usiTwiSlave.h"


#ifdef __AVR_ATtiny85__
#define TWI_SLAVE_ADDRESS	0x10
#endif


void TWI__loop (void (*data_callback_in)(uint8_t input_buffer_length, const uint8_t *input_buffer, uint8_t *output_buffer_length, uint8_t *output_buffer));


#endif /* TWI_H_ */
