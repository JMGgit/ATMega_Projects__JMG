/*
 * Mode_LEDScreen.c
 *
 *  Created on: 31.05.2016
 *      Author: Jean-Martin George
 */


#include "Mode_LEDScreen.h"


#define CMD_NEW_DATA 1
#define USART1_DATA_LENGTH	((3 * LEDS_NB))
#define USART1_FIRST_BYTE	1

volatile uint8_t USARTdata_ready = FALSE;
uint16_t USART1_data[USART1_DATA_LENGTH];
static volatile uint16_t USART1_idxData = 0;
extern volatile uint8_t USARTdata_ready;
extern uint16_t USART1_data[((3 * LEDS_NB))];
uint8_t volatile ledTableUSARTmode;


void LEDScreen__loop (void)
{
	uint16_t idx = 0;

	if (USARTdata_ready)
	{
		for (idx = 0; idx < LEDS_NB * 3; idx++)
		{
			SPDR = USART1_data[idx]; while (!(SPSR & (1 << SPIF))) {};
		}

		USARTdata_ready = FALSE;
		/* assuming USARTdata_ready is long enough FALSE (depending on LED drivers)
		 * Otherwise delay should be implemented */
		//toggle(TEST2_LED_PORT, TEST2_LED_PIN);
		wdt_reset();
	}
}


void LEDScreen__reset (void)
{
	USART1_idxData = 0;
	ledTableUSARTmode = FALSE;
}

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega1284P__)
ISR(USART1_RX_vect)
{
	if ((USART1_idxData == 0) && (UDR1 == USART1_FIRST_BYTE))
	{
		USART1_idxData++;
		ledTableUSARTmode = TRUE;
		//toggle(TEST2_LED_PORT, TEST2_LED_PIN);
	}
	else
	{
		USART1_data[USART1_idxData - 1] = UDR1;

		if (USART1_idxData < (USART1_DATA_LENGTH))
		{
			USART1_idxData++;
		}
		else
		{
			USART1_idxData = 0;
			USARTdata_ready = TRUE;
		}
	}
}
#endif
