/*
  * 74HC165.c
 *
 *  Created on: 02.03.2013
 *      Author: Jean-Martin George
 */


#include "HC165.h"


#if (BUTTONS_WIRED == BUTTONS_WIRED_HC165)

void HC165__init (void)
{
	setOutput(HC165_CLK_DDR, HC165_CLK_PIN);
	setOutput(HC165_SL_DDR, HC165_SL_PIN);
	setInput(HC165_OUT_DDR, HC165_OUT_PIN);
	setLow(HC165_CLK_PORT, HC165_CLK_PIN);
	setLow(HC165_SL_PORT, HC165_SL_PIN);
}

void HC165__readByte (uint8_t *buffer)
{
	uint8_t i;

	*buffer = 0;

	/* shift data to the internal register */
	setHigh(HC165_CLK_PORT, HC165_CLK_PIN);

	/* disable inputs */
	setHigh(HC165_SL_PORT, HC165_SL_PIN);

	/* read the output */
	for (i = 0; i < 8; i++)
	{
		if (isHigh(HC165_OUT_PORT, HC165_OUT_PIN))
		{
			*buffer |= (1 << i);
		}
		setLow(HC165_CLK_PORT, HC165_CLK_PIN);
		setHigh(HC165_CLK_PORT, HC165_CLK_PIN);
	}

	/* enable inputs */
	setLow(HC165_SL_PORT, HC165_SL_PIN);
}

#endif
