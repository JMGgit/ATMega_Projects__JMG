/*
 * ADC.c
 *
 *  Created on: 18.01.2014
 *      Author: JMG
 */


#include "ADC.h"


void ADC__init (void)
{
	ADMUX = (1 << REFS0); /* AVCC */
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	/* start first conversion */
	ADCSRA |= (1 << ADSC);

	Debug__setWhileState(WHILE_STATE_ADC1_BEFORE);
	while (ADCSRA & (1 << ADSC))
	{
		/* wait */
	}
	Debug__setWhileState(WHILE_STATE_ADC1_AFTER);

	(void)ADCL;
	(void)ADCH;
}


uint16_t ADC__readValue (uint8_t channel)
{
	uint16_t retVal;

	if (channel <= 7)
	{
		ADMUX &= ~ (0x1F);
		ADMUX |= channel;

		/* start conversion */
		ADCSRA |= (1 << ADSC);

		Debug__setWhileState(WHILE_STATE_ADC2_BEFORE);
		while (ADCSRA & (1 << ADSC))
		{
			/* wait */
		}
		Debug__setWhileState(WHILE_STATE_ADC2_AFTER);

		retVal = ((ADCL) | (ADCH << 8));
	}
	else
	{
		retVal = 0;
	}

	return retVal;
}
