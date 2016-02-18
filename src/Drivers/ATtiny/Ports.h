/*
 * Ports.h
 *
 *  Created on: 05.02.2016
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

#define pulse(port, pin)	\
	do						\
	{						\
		setHigh(port, pin);	\
		setLow(port, pin);	\
	} while (0)

#define toggle(port, pin)  (port ^= (1 << pin))


#endif /* PORTS_H_ */
