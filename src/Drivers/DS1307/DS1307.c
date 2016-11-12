/*
 * DS1307.c
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */


#include "DS1307.h"


#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)

static DS1307_time currentTime;

static uint8_t triggerUpdateToRTC;
static uint8_t updateFromRTCDone;
static uint8_t triggerUpdateFromRTC;
#if (DS1307_MODE == DS1307_MODE_TWI_SQW)
static uint8_t daysInYear[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static uint8_t daysInLeapYear[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
#endif


static void DS1307__sendTimeToRTC (void)
{
	uint8_t data[8];
	uint8_t transmitState = E_NOT_OK;

	data[0] = 0x00;
	data[1] = (((currentTime.seconds / 10) << 4) & (0x70)) | ((currentTime.seconds % 10) & 0x0F);
	data[2] = (((currentTime.minutes / 10) << 4) & (0x70)) | ((currentTime.minutes % 10) & 0x0F);
	data[3] = (((currentTime.hours / 10) << 4) & (0x30)) | ((currentTime.hours % 10) & 0x0F); /* bit 6 is not set -> 24-hour mode */
	data[4] = (currentTime.day % 10) & 0x07;
	data[5] = (((currentTime.date / 10) << 4) & (0x30)) | ((currentTime.date % 10) & 0x0F);
	data[6] = (((currentTime.month / 10) << 4) & (0x10)) | ((currentTime.month % 10) & 0x0F);
	data[7] = (((currentTime.year / 10) << 4) & (0xF0)) | ((currentTime.year % 10) & 0x0F);

	Debug__setWhileState(WHILE_STATE_DS13075_BEFORE);
    while (transmitState != E_OK)
    {
        transmitState = TWI__masterTransmitData(data, 8, DS1307_ADDRESS);
    }
    Debug__setWhileState(WHILE_STATE_DS13075_AFTER);

	transmitState = E_NOT_OK;
}


void DS1307__init (void)
{
	uint8_t data[2];
	uint8_t transmitState = E_NOT_OK;
	uint8_t receiveState = E_NOT_OK;

	/* set clock register */
	data[0] = 0x00;

	Debug__setWhileState(WHILE_STATE_DS13071_BEFORE);
	while (transmitState != E_OK)
	{
	    transmitState = TWI__masterTransmitData(data, 1, DS1307_ADDRESS);
	}
	Debug__setWhileState(WHILE_STATE_DS13071_AFTER);

	transmitState = E_NOT_OK;

	/* read first byte */
	Debug__setWhileState(WHILE_STATE_DS13072_BEFORE);
	while (receiveState != E_OK)
	{
		receiveState = TWI__masterReadData (&data[1], 1, DS1307_ADDRESS);
	}
	Debug__setWhileState(WHILE_STATE_DS13072_AFTER);

	receiveState = E_NOT_OK;

	/* enable oscillator */
	data[1] &= (~(1 << 7));

	Debug__setWhileState(WHILE_STATE_DS13073_BEFORE);
	while (transmitState != E_OK)
    {
	    transmitState = TWI__masterTransmitData(data, 2, DS1307_ADDRESS);
    }
	Debug__setWhileState(WHILE_STATE_DS13073_AFTER);

	transmitState = E_NOT_OK;

	/* set control register and enable SQW output - 1 Hz */
	data[0] = 0x07;
	data[1] = (1 << 4);

	Debug__setWhileState(WHILE_STATE_DS13074_BEFORE);
    while (transmitState != E_OK)
    {
        transmitState = TWI__masterTransmitData(data, 2, DS1307_ADDRESS);
    }
    Debug__setWhileState(WHILE_STATE_DS13074_AFTER);

    transmitState = E_NOT_OK;

    DS1307__sendTimeToRTC();

#if (DS1307_MODE == DS1307_MODE_TWI_SQW)
    setInput(DS1307_SQW_DDR, DS1307_SQW_PIN);
	/* !! For TWI+SQW mode Pin PC7 has to be used !! */
    PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT23);
#endif

	triggerUpdateFromRTC = TRUE;
}


void DS1307__triggerUpdateToRTC (void)
{
	triggerUpdateToRTC = TRUE;
}


static uint8_t DS1307__isUpdateFromRTCRequired (void)
{
#if (DS1307_MODE == DS1307_MODE_TWI)
	updateFromRTCDone = FALSE;
	return TRUE;
#else
	uint8_t update = FALSE;

	if (triggerUpdateFromRTC == TRUE)
	{
		update = TRUE;
		triggerUpdateFromRTC = FALSE;
	}
	else
	{
		if (updateFromRTCDone  == TRUE)
		{
			if (	(DS1307__getHours() == DS1307_UPDATE_HOUR)
				||	(DS1307__getMinutes() == DS1307_UPDATE_MIN)
				||	(DS1307__getSeconds() == DS1307UPDATE_SEC)
			)
			{
				updateFromRTCDone = FALSE;
			}
		}
		else
		{
			if (	(DS1307__getHours() == DS1307_UPDATE_HOUR)
				&&	(DS1307__getMinutes() == DS1307_UPDATE_MIN)
				&&	(DS1307__getSeconds() == DS1307UPDATE_SEC)
			)
			{
				update = TRUE;
			}
		}
	}

	return update;
#endif
}


static void DS1307__updateTimeFromRTC (void)
{
	uint8_t data[8];
	uint8_t transmitState = E_NOT_OK;
	uint8_t receiveState = E_NOT_OK;

	/* set clock register and read bytes */
	data[0] = 0x00;

	Debug__setWhileState(WHILE_STATE_DS13076_BEFORE);
	while (transmitState != E_OK)
	{
		transmitState = TWI__masterTransmitData(data, 1, DS1307_ADDRESS);
	}
	Debug__setWhileState(WHILE_STATE_DS13076_AFTER);

	if (transmitState == E_OK)
	{
		Debug__setWhileState(WHILE_STATE_DS13077_BEFORE);
		while (receiveState != E_OK)
		{
			receiveState = TWI__masterReadData(&data[1], 7, DS1307_ADDRESS);
		}
		Debug__setWhileState(WHILE_STATE_DS13077_AFTER);

		/* copy to buffer */
		currentTime.seconds = (((data[1] & 0x70) >> 4) * 10) + (data[1] & 0x0F);
		currentTime.minutes = (((data[2] & 0x70) >> 4) * 10) + (data[2] & 0x0F);
		currentTime.hours = (((data[3] & 0x30) >> 4) * 10) + (data[3] & 0x0F);
		currentTime.day = data[4] & 0x07;
		currentTime.date = (((data[5] & 0x30) >> 4) * 10) + (data[5] & 0x0F);
		currentTime.month = (((data[6] & 0x10) >> 4) * 10) + (data[6] & 0x0F);
		currentTime.year = (((data[7] & 0xF0) >> 4) * 10) + (data[7] & 0x0F);

		transmitState = E_NOT_OK;
		receiveState = E_NOT_OK;
	}
}


void DS1307__x10 (void)
{
	if ((DS1307__isUpdateFromRTCRequired()) == TRUE)
	{
		DS1307__updateTimeFromRTC();
		updateFromRTCDone = TRUE;
	}

	if ((triggerUpdateToRTC) == TRUE)
	{
		DS1307__sendTimeToRTC();
		triggerUpdateFromRTC = FALSE;
		triggerUpdateFromRTC = TRUE; /* get time from RTC to ensure that transmitted time is correct */
	}
}


#if (DS1307_MODE == DS1307_MODE_TWI_SQW)
static uint8_t DS1307__isLeapYear (uint16_t year)
{
	if (((currentTime.year % 400) == 0) || (((currentTime.year % 100) != 0) && ((currentTime.year % 4) == 0)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


static uint8_t DS1307__getNumberOfDays (uint16_t year, uint8_t month)
{
	if (DS1307__isLeapYear(year))
	{
		return daysInLeapYear[month];
	}
	else
	{
		return daysInYear[month];
	}
}


static void DS1307__incYear (void)
{
	if (currentTime.year < 99)
	{
		currentTime.year++;
	}
	else
	{
		currentTime.year = 0;
	}
}


static void DS1307__incMonth (void)
{
	if (currentTime.month < 12)
	{
		currentTime.month++;
	}
	else
	{
		currentTime.month = 1;
		DS1307__incYear();
	}
}


static void DS1307__incDay (void)
{
	if (currentTime.day < 7)
	{
		currentTime.day++;
	}
	else
	{
		currentTime.day = 1;
	}
}


static void DS1307__incDate (void)
{
	if (currentTime.date < DS1307__getNumberOfDays(currentTime.year + 2000, currentTime.month))
	{
		currentTime.date++;
		DS1307__incDay();
	}
	else
	{
		currentTime.date = 1;
		DS1307__incMonth();
	}
}


static void DS1307__incHours (void)
{
	if (currentTime.hours < 23)
	{
		currentTime.hours++;
	}
	else
	{
		currentTime.hours = 0;
		DS1307__incDate();
	}
}


static void DS1307__incMinutes (void)
{
	if (currentTime.minutes < 59)
	{
		currentTime.minutes++;
	}
	else
	{
		currentTime.minutes = 0;
		DS1307__incHours();
	}

}


static void DS1307__incSeconds (void)
{
	if (currentTime.seconds < 59)
	{
		currentTime.seconds++;
	}
	else
	{
		currentTime.seconds = 0;
		DS1307__incMinutes();
	}
}


ISR(PCINT2_vect)
{
	if (isHigh(DS1307_SQW_PORT, DS1307_SQW_PIN))
	{
		DS1307__incSeconds();
	}
}
#endif


uint8_t DS1307__getSeconds (void)			{return currentTime.seconds;}
uint8_t DS1307__getMinutes (void)			{return currentTime.minutes;}
uint8_t DS1307__getHours (void)				{return currentTime.hours;}
uint8_t DS1307__getDay (void)				{return currentTime.day;}
uint8_t DS1307__getDate (void)				{return currentTime.date;}
uint8_t DS1307__getMonth (void)				{return currentTime.month;}
uint8_t DS1307__getYear (void)				{return currentTime.year;}

void DS1307__setSeconds (uint8_t seconds)	{currentTime.seconds = seconds;}
void DS1307__setMinutes (uint8_t minutes)	{currentTime.minutes = minutes;}
void DS1307__setHours (uint8_t hours)		{currentTime.hours = hours;}
void DS1307__setDay (uint8_t day)			{currentTime.day = day;}
void DS1307__setDate (uint8_t date)			{currentTime.date = date;}
void DS1307__setMonth (uint8_t month)		{currentTime.month = month;}
void DS1307__setYear (uint8_t year)			{currentTime.year = year;}

#endif
