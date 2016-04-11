/*
 * Ports.h
 *
 *  Created on: 28.03.2013
 *      Author: Jean-Martin George
 */

#ifndef PORTS_H_
#define PORTS_H_


#define setInput(ddr, pin)		(ddr &= ~(1 << pin))
#define isLow(port, pin)		((port & (1 << pin)) == 0)
#define isHigh(port, pin)		((port & (1 << pin)) != 0)

#define setOutput(ddr, pin)		(ddr |= (1 << pin))
#define setLow(port, pin)		(port &= ~(1 << pin))
#define setHigh(port, pin)		(port |= (1 << pin))

#define toggle(port, pin)  (port ^= (1 << pin))


#endif /* PORTS_H_ */
