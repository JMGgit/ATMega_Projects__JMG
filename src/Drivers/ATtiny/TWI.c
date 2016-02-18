/*
 * TWI.c
 *
 *  Created on: 05.02.2016
 *      Author: Jean-Martin George
 */


#include "TWI.h"


void TWI__loop (void (*data_callback_in)(uint8_t input_buffer_length, const uint8_t *input_buffer, uint8_t *output_buffer_length, uint8_t *output_buffer))
{
	usi_twi_slave(TWI_SLAVE_ADDRESS, 0, data_callback_in, NULL);
}
