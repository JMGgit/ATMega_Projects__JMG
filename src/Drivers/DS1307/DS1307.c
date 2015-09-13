/*
 * DS1307.c
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */


#include "DS1307.h"


#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)

static DS1307_time currentTime;


void DS1307__init (void)
{
	uint8_t data[2];
	uint8_t transmitState = E_NOT_OK;
	uint8_t receiveState = E_NOT_OK;

	/* set clock register */
	data[0] = 0x00;

	while (transmitState != E_OK)
	{
	    transmitState = TWI__transmitData(data, 1, DS1307_ADDRESS);
	}

	transmitState = E_NOT_OK;

	/* read first byte */
	while (receiveState != E_OK)
	{
		receiveState = TWI__readData (&data[1], 1, DS1307_ADDRESS);
	}

	receiveState = E_NOT_OK;

	/* enable oscillator */
	data[1] &= (~(1 << 7));

	while (transmitState != E_OK)
    {
	    transmitState = TWI__transmitData(data, 2, DS1307_ADDRESS);
    }

	transmitState = E_NOT_OK;

	/* set control register and enable SQW output - 1 Hz */
	data[0] = 0x07;
	data[1] = (1 << 4);

    while (transmitState != E_OK)
    {
        transmitState = TWI__transmitData(data, 2, DS1307_ADDRESS);
    }

    transmitState = E_NOT_OK;

	/* update time from RTC */
	DS1307__updateTimeFromRTC();
}


void DS1307__sendTimeToRTC (void)
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

    while (transmitState != E_OK)
    {
        transmitState = TWI__transmitData(data, 8, DS1307_ADDRESS);
    }

	transmitState = E_NOT_OK;
}


void DS1307__updateTimeFromRTC (void)
{
	uint8_t data[8];
	static uint8_t transmitState = E_NOT_OK;
	static uint8_t receiveState = E_NOT_OK;

	/* set clock register and read bytes */
	data[0] = 0x00;

	if (transmitState != E_OK)
	{
	    transmitState = TWI__transmitData(data, 1, DS1307_ADDRESS);
	}

	if (transmitState == E_OK)
	{
	    while (receiveState != E_OK)
	    {
	        receiveState = TWI__readData(&data[1], 7, DS1307_ADDRESS);
	    }

	    if (receiveState == E_OK)
	    {
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
}


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
