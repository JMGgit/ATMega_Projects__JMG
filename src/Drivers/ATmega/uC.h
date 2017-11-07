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
#include <avr/wdt.h>
#include "Ports.h"
#include "SPI.h"
#include "TWI_Master.h"
#include "TWI_Slave.h"
#include "USART.h"
#include "ADC.h"
#include <util/atomic.h>
#include "Main_Types.h"
#include "Drivers_Config.h"


#ifndef F_CPU
#error F_CPU not defined!
#endif

void uC__init (void);
uint8_t uC__isTaskTrigger_x10 (void);
void uC__resetTaskTrigger_x10 (void);
void uC__triggerSwReset (void);
void uC__begin_x10 (void);
void uC__end_x10 (void);

void uC__enableWatchdog (void);
void uC__disableWatchdog (void);

#if (PROJECT != PROJECT__IRMP)
extern uint8_t Modes__getTaskIncrement (void);
#else
static inline uint8_t Modes__getTaskIncrement (void)
{
	return 1;
}
#endif


static inline uint8_t uC__getTaskIncrement (void)
{
	return Modes__getTaskIncrement();
}


#endif /* UC_H_ */
