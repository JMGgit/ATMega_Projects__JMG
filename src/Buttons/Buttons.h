/*
 * Buttons.h
 *
 *  Created on: 02.03.2013
 *      Author: Jean-Martin George
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_


#include "Main_Types.h"
#include "Main_Config.h"
#include "Drivers.h"


#define BITPOS_BUTTON_OFF		8
#define BITPOS_BUTTON_MODE		7
#define BITPOS_BUTTON_FUNC1		6
#define BITPOS_BUTTON_FUNC2		5
#define BITPOS_BUTTON_FUNC3		4
#define BITPOS_BUTTON_LEFT		3
#define BITPOS_BUTTON_UP		2
#define BITPOS_BUTTON_DOWN		1
#define BITPOS_BUTTON_RIGHT		0

#define BITMSK_BUTTON_OFF		(1 << BITPOS_BUTTON_OFF)
#define BITMSK_BUTTON_MODE		(1 << BITPOS_BUTTON_MODE)
#define BITMSK_BUTTON_FUNC1		(1 << BITPOS_BUTTON_FUNC1)
#define BITMSK_BUTTON_FUNC2		(1 << BITPOS_BUTTON_FUNC2)
#define BITMSK_BUTTON_FUNC3		(1 << BITPOS_BUTTON_FUNC3)
#define BITMSK_BUTTON_LEFT		(1 << BITPOS_BUTTON_LEFT)
#define BITMSK_BUTTON_UP		(1 << BITPOS_BUTTON_UP)
#define BITMSK_BUTTON_DOWN		(1 << BITPOS_BUTTON_DOWN)
#define BITMSK_BUTTON_RIGHT		(1 << BITPOS_BUTTON_RIGHT)

#define BUTTON_OFF				BITMSK_BUTTON_OFF
#define BUTTON_MODE				BITMSK_BUTTON_MODE
#define BUTTON_FUNC1			BITMSK_BUTTON_FUNC1
#define BUTTON_FUNC2			BITMSK_BUTTON_FUNC2
#define BUTTON_FUNC3			BITMSK_BUTTON_FUNC3
#define BUTTON_LEFT				BITMSK_BUTTON_LEFT
#define BUTTON_UP				BITMSK_BUTTON_UP
#define BUTTON_DOWN				BITMSK_BUTTON_DOWN
#define BUTTON_RIGHT			BITMSK_BUTTON_RIGHT


#define BUTTON_DEBOUNCE_TIME	2
#define IRMP_DEBOUNCE_TIME      10

#define BUTTON_INVALID 			0x00

#define Button__isLow(port, pin)	((pin != BUTTON_INVALID) && isLow(port, pin))

#define Button__initButton(ddr, port, pin)	\
{											\
	if (pin != BUTTON_INVALID)				\
	{										\
		setInput(ddr, pin);					\
		setHigh(port, pin);					\
	}										\
}

#if (BUTTONS_TWI == BUTTONS_TWI_ON)
#define BUTTONS_TWI_SLAVE_ADDRESS	0x10
#endif


typedef struct
{
	uint16_t bitMask;
	uint8_t debounceTime;
	uint8_t pressed;
	uint8_t pressedOnce;
#if (BUTTONS_USART == BUTTONS_USART_ON)
	uint8_t pressedUSART;
	uint8_t pressedOnceUSART;
#endif
#if (BUTTONS_IRMP != BUTTONS_IRMP_OFF)
	uint8_t pressedIRMP;
	uint8_t debounceTimeIRMP;
	uint8_t pressedOnceIRMP;
#endif
#if (BUTTONS_TWI == BUTTONS_TWI_ON)
	uint8_t pressedTWI;
	uint8_t pressedOnceTWI;
#endif
} Button_t;

extern Button_t buttonOff;
extern Button_t buttonMode;
extern Button_t buttonFunc1;
extern Button_t buttonFunc2;
extern Button_t buttonFunc3;
extern Button_t buttonLeft;
extern Button_t buttonRight;
extern Button_t buttonUp;
extern Button_t buttonDown;

void Buttons__init (void);
void Buttons__x10 (void);

uint8_t Buttons__isPressed (Button_t *button);
uint8_t Buttons__isPressedOnce (Button_t *button);
#if (BUTTONS_IRMP == BUTTONS_IRMP_SEND_TO_TWI)
void Buttons__TwiDataCallback (uint8_t input_buffer_length, const uint8_t *input_buffer, uint8_t *output_buffer_length, uint8_t *output_buffer);
#endif
#endif /* BUTTONS_H_ */
