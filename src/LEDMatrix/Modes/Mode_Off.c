/*
 * Mode_Off.c
 *
 *  Created on: 07.04.2016
 *      Author: Jean-Martin George
 */


#include "Mode_Off.h"
#include "Modes.h"
#if (PROJECT == PROJECT__QLOCKTWO)
#include "Modes_Qlocktwo/Modes_Qlocktwo.h"
#endif


void Off__x10 (void)
{
#if (BUTTON_FUNC3_AVAILABLE == BUTTON_FUNC3_AVAILABLE_YES)
	static uint8_t rgbConnectionTimer = 255;
	static uint8_t ledConnectionTimer = 255;
	static uint8_t startupTimer = 255;
#if (PROJECT == PROJECT__QLOCKTWO)
	static uint8_t langTimer = 255;
#endif
#endif

#if (BUTTON_OFF_AVAILABLE == BUTTON_OFF_AVAILABLE_FUNC2)
	if (Buttons__isPressedOnce(&buttonFunc2))
	{
		Modes__Start();
	}
#endif

	if (Buttons__isPressedOnce(&buttonOff))
	{
		Modes__Start();
	}

#if (BUTTON_FUNC3_AVAILABLE == BUTTON_FUNC3_AVAILABLE_YES)
	if (Buttons__isPressed(&buttonFunc1))
	{
		if (rgbConnectionTimer > 0)
		{
			rgbConnectionTimer--;
		}
		else
		{
			rgbConnectionTimer = 255;
			LEDMatrix__toggleRGBLedOrder();
			uC__triggerSwReset();
		}
	}
	else
	{
		rgbConnectionTimer = 255;
	}

	if (Buttons__isPressed(&buttonFunc2))
	{
		if (ledConnectionTimer > 0)
		{
			ledConnectionTimer--;
		}
		else
		{
			ledConnectionTimer = 255;
			LEDMatrix__toggleLedOrder();
			uC__triggerSwReset();
		}
	}
	else
	{
		ledConnectionTimer = 255;
	}

	if (Buttons__isPressed(&buttonFunc3))
	{
		if (startupTimer > 0)
		{
			startupTimer--;
		}
		else
		{
			startupTimer = 255;
			Modes__toggleStartupMode();
			uC__triggerSwReset();
		}
	}
	else
	{
		startupTimer = 255;
	}

#if (PROJECT == PROJECT__QLOCKTWO)
	if (Buttons__isPressed(&buttonMode))
	{
		if (langTimer > 0)
		{
			langTimer--;
		}
		else
		{
			langTimer = 255;
			Qtwo__setNextLang();
			uC__triggerSwReset();
		}
	}
	else
	{
		langTimer = 255;
	}
#endif
#endif
}
