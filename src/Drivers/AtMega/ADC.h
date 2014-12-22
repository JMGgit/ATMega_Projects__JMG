/*
 * ADC.h
 *
 *  Created on: 18.01.2014
 *      Author: JMG
 */

#ifndef ADC_H_
#define ADC_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"
#include "Ports.h"


void ADC__init (void);
uint16_t ADC__readValue (uint8_t channel);

#endif /* ADC_H_ */
