/*
 * Clock.c
 *
 *  Created on: 12.03.2013
 *      Author: Jean-Martin George
 */


#include "Clock.h"


#if (CLOCK_TYPE != CLOCK_TYPE_OFF)

static Clock_time currentTime;
#if (CLOCK_SYNC != CLOCK_SYNC_OFF)
static Clock_time currentSyncTime;
#endif
#if (CLOCK_SYNC == CLOCK_SYNC_DCF77)
static Clock_time previousSyncTime;
static uint8_t currentSyncMinutesCheck;
static uint8_t previousSyncMinutesCheck;
static uint8_t currentDS1307MinutesCheck;
static uint8_t previousDS1307MinutesCheck;
#endif


static void Clock__updateTimeFromRTC (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	currentTime.seconds = DS1307__getSeconds();
	currentTime.minutes = DS1307__getMinutes();
	currentTime.hours   = DS1307__getHours();
	currentTime.day     = DS1307__getDay();
	currentTime.date    = DS1307__getDate();
	currentTime.month   = DS1307__getMonth();
	currentTime.year    = DS1307__getYear();

#endif
}

#if (CLOCK_SYNC == CLOCK_SYNC_DCF77)

static void Clock__getCurrentTimeFromDCF77 (void)
{
	/* save last signal */
	previousSyncTime.seconds  = currentSyncTime.seconds;
	previousSyncTime.minutes  = currentSyncTime.minutes;
	previousSyncTime.hours    = currentSyncTime.hours;
	previousSyncTime.day      = currentSyncTime.day;
	previousSyncTime.date     = currentSyncTime.date;
	previousSyncTime.month    = currentSyncTime.month;
	previousSyncTime.year     = currentSyncTime.year;
	previousSyncMinutesCheck  = currentSyncMinutesCheck;
	previousDS1307MinutesCheck = currentDS1307MinutesCheck;

	/* copy new signal */
	currentSyncTime.seconds  = 0;
	currentSyncTime.minutes  = DCF77__getMinutes();
	currentSyncTime.hours    = DCF77__getHours();
	currentSyncTime.day      = DCF77__getDay();
	currentSyncTime.date     = DCF77__getDate();
	currentSyncTime.month    = DCF77__getMonth();
	currentSyncTime.year     = DCF77__getYear();
	currentSyncMinutesCheck  = DCF77__getMinutes();
	currentDS1307MinutesCheck = DS1307__getMinutes();
}


static uint8_t Clock__isLastDCF77SignalPlausiCheckOk (void)
{
	uint8_t retVal = FALSE;

	if (currentSyncMinutesCheck > previousSyncMinutesCheck)
	{
		if (	(currentSyncMinutesCheck - previousSyncMinutesCheck)
			== 	(currentDS1307MinutesCheck - previousDS1307MinutesCheck)
		)
		{
			retVal = TRUE;
		}
		else
		{
			retVal = FALSE;
		}
	}
	else if (currentSyncMinutesCheck < previousSyncMinutesCheck)
	{
		if (	(previousSyncMinutesCheck - currentSyncMinutesCheck)
			== 	(previousDS1307MinutesCheck - currentDS1307MinutesCheck)
		)
		{
			retVal = TRUE;
		}
		else
		{
			retVal = FALSE;
		}
	}
	else
	{
		/* both signals = 0 -> do not send */
		retVal = FALSE;
	}

	return retVal;
}


static void Clock__resetSyncTime (void)
{
	previousSyncTime.seconds  = 0;
	previousSyncTime.minutes  = 0;
	previousSyncTime.hours    = 0;
	previousSyncTime.day      = 0;
	previousSyncTime.date     = 0;
	previousSyncTime.month    = 0;
	previousSyncTime.year     = 0;
	previousSyncMinutesCheck  = 0;
	previousDS1307MinutesCheck = 0;

	currentSyncTime.seconds  = 0;
	currentSyncTime.minutes  = 0;
	currentSyncTime.hours    = 0;
	currentSyncTime.day      = 0;
	currentSyncTime.date     = 0;
	currentSyncTime.month    = 0;
	currentSyncTime.year     = 0;
	currentSyncMinutesCheck  = 0;
	currentDS1307MinutesCheck = 0;
}

#endif


#if (CLOCK_SYNC != CLOCK_SYNC_OFF)
static void Clock__sendCurrentSyncTimeToRTC (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (        (currentSyncTime.seconds != DS1307__getSeconds())
	        ||  (currentSyncTime.minutes != DS1307__getMinutes())
	        ||  (currentSyncTime.hours != DS1307__getHours())
	        ||  (currentSyncTime.day != DS1307__getDay())
	        ||  (currentSyncTime.date != DS1307__getDate())
	        ||  (currentSyncTime.month != DS1307__getMonth())
	        ||  (currentSyncTime.year != DS1307__getYear())
	)
    {
		DS1307__setSeconds(currentSyncTime.seconds);
		DS1307__setMinutes(currentSyncTime.minutes);
		DS1307__setHours(currentSyncTime.hours );
		DS1307__setDay(currentSyncTime.day);
		DS1307__setDate(currentSyncTime.date);
		DS1307__setMonth(currentSyncTime.month);
		DS1307__setYear(currentSyncTime.year);

        DS1307__triggerUpdateToRTC();
    }
#endif
}
#endif


void Clock__incDate (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)

	if (DS1307__getDate() < 31)
	{
		DS1307__setDate(DS1307__getDate() + 1);
	}
	else
	{
		DS1307__setDate(1);
	}

	DS1307__triggerUpdateToRTC();

#endif
}


void Clock__incMonth (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)

	if (DS1307__getMonth() < 12)
	{
		DS1307__setMonth(DS1307__getMonth() + 1);
	}
	else
	{
		DS1307__setMonth(1);
	}

	DS1307__triggerUpdateToRTC();

#endif
}


void Clock__incYear (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (DS1307__getYear() < 99)
	{
		DS1307__setYear(DS1307__getYear() + 1);
	}
	else
	{
		DS1307__setYear(0);
	}

	DS1307__triggerUpdateToRTC();
#endif
}


void Clock__incHours (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (DS1307__getHours() < 23)
	{
		DS1307__setHours(DS1307__getHours() + 1);
	}
	else
	{
		DS1307__setHours(0);
	}

	DS1307__triggerUpdateToRTC();
#endif
}


void Clock__incMinutes (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (DS1307__getMinutes() < 59)
	{
		DS1307__setMinutes(DS1307__getMinutes() +  1);
	}
	else
	{
		DS1307__setMinutes(0);
	}

	DS1307__triggerUpdateToRTC();
#endif
}


void Clock__incSeconds (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (DS1307__getSeconds() < 59)
	{
		DS1307__setSeconds(DS1307__getSeconds() +  1);
	}
	else
	{
		DS1307__setSeconds(0);
	}

	DS1307__triggerUpdateToRTC();
#endif
}


void Clock__decDate (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (DS1307__getDate() > 0)
	{
		DS1307__setDate(DS1307__getDate() - 1);
	}
	else
	{
		DS1307__setDate(31);
	}

	DS1307__triggerUpdateToRTC();
#endif
}


void Clock__decMonth (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (DS1307__getMonth() > 0)
	{
		DS1307__setMonth(DS1307__getMonth() - 1);
	}
	else
	{
		DS1307__setMonth(12);
	}

	DS1307__triggerUpdateToRTC();
#endif
}


void Clock__decYear (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (DS1307__getYear() > 0)
	{
		DS1307__setYear(DS1307__getYear() - 1);
	}
	else
	{
		DS1307__setYear(99);
	}

	DS1307__triggerUpdateToRTC();
#endif
}


void Clock__decHours (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (DS1307__getHours() > 0)
	{
		DS1307__setHours(DS1307__getHours() - 1);
	}
	else
	{
		DS1307__setHours(23);
	}

	DS1307__triggerUpdateToRTC();
#endif
}


void Clock__decMinutes (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (DS1307__getMinutes() > 0)
	{
		DS1307__setMinutes(DS1307__getMinutes() -  1);
	}
	else
	{
		DS1307__setMinutes(59);
	}

	DS1307__triggerUpdateToRTC();
#endif
}


void Clock__decSeconds (void)
{
#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)
	if (DS1307__getSeconds() > 0)
	{
		DS1307__setSeconds(DS1307__getSeconds() -  1);
	}
	else
	{
		DS1307__setSeconds(59);
	}

	DS1307__triggerUpdateToRTC();
#endif
}


static uint8_t Clock__updateSyncSignal (void)
{
	uint8_t retVal = CLOCK_SYNC_NOT_OK;

#if (CLOCK_SYNC == CLOCK_SYNC_DCF77)
	if ((DCF77_STATUS_OK == DCF77__updateSignal()))
	{
		retVal = CLOCK_SYNC_OK;
	}
#elif (CLOCK_SYNC == CLOCK_SYNC_USART)
	uint8_t USARTbuffer[USART_DATA_LENGTH_CLOCK];

	if (E_OK == USART__readData(USARTbuffer, USART_DATA_LENGTH_CLOCK, USART_REQESTER_CLOCK))
	{
		currentSyncTime.seconds	= USARTbuffer[1];
		currentSyncTime.minutes	= USARTbuffer[2];
		currentSyncTime.hours	= USARTbuffer[3];
		currentSyncTime.day		= USARTbuffer[4];
		currentSyncTime.date	= USARTbuffer[5];
		currentSyncTime.month	= USARTbuffer[6];
		currentSyncTime.year	= USARTbuffer[7];
		retVal = CLOCK_SYNC_OK;
	}
#endif

	return retVal;
}


void Clock__init (void)
{
	setOutput(CLOCK_LED_DDR, CLOCK_LED_PIN);
	Clock__x10(); /* update time */
}


void Clock__x10 (void)
{
	uint8_t clockSyncStatus = 0xFF;

	/* test */
	static uint8_t test_led = 0;
	static uint8_t testSeconds = 0;

	/* DS1307 */
	Clock__updateTimeFromRTC();

	/* update synchronization signal */
	clockSyncStatus = Clock__updateSyncSignal();

	if (clockSyncStatus == CLOCK_SYNC_OK)
	{
#if (CLOCK_SYNC == CLOCK_SYNC_DCF77)
		Clock__getCurrentTimeFromDCF77();

		if (Clock__isLastDCF77SignalPlausiCheckOk())
		{
			Clock__sendCurrentSyncTimeToRTC();
			Clock__resetSyncTime();
		}
#elif (CLOCK_SYNC == CLOCK_SYNC_USART)
		Clock__sendCurrentSyncTimeToRTC();
#endif
	}

	if (test_led - uC__getTaskIncrement() > 0)
	{
		test_led = test_led - uC__getTaskIncrement();
		setHigh(CLOCK_LED_PORT, CLOCK_LED_PIN);
	}
	else
	{
		setLow(CLOCK_LED_PORT, CLOCK_LED_PIN);
	}

	/* Test */
	if (testSeconds != Clock__getSeconds())
	{
		/* LED */
		test_led = 10;
		testSeconds = Clock__getSeconds();
	}
}


uint8_t Clock__getSeconds (void)		{return currentTime.seconds;}
uint8_t Clock__getMinutes (void)		{return currentTime.minutes;}
uint8_t Clock__getHours (void)			{return currentTime.hours;}
uint8_t Clock__getDay (void)			{return currentTime.day;}
uint8_t Clock__getDate (void)			{return currentTime.date;}
uint8_t Clock__getMonth (void)			{return currentTime.month;}
uint8_t Clock__getYear (void)			{return currentTime.year;}


void Clock__getHourString (char* buffer)
{
	uint8_t hours;
	uint8_t idxBuffer = 0;

	hours = Clock__getHours();

	if (hours >= 10)
	{
		itoa(hours, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(hours, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ' ';
}


void Clock__getMinutesString (char* buffer)
{
	uint8_t minutes;
	uint8_t idxBuffer = 0;

	minutes = Clock__getMinutes();

	if (minutes >= 10)
	{
		itoa(minutes, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(minutes, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ' ';
}


void Clock__getTimeString (char* buffer)
{
	uint8_t hours, minutes;
	uint8_t idxBuffer = 0;

	hours = Clock__getHours();
	minutes = Clock__getMinutes();

	if (hours >= 10)
	{
		itoa(hours, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(hours, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ':';

	if (minutes >= 10)
	{
		itoa(minutes, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(minutes, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ' ';
}


void Clock__convertTimeWithSecondsToString (uint8_t hours, uint8_t minutes, uint8_t seconds, char* buffer)
{
	uint8_t idxBuffer = 0;

	if (hours >= 10)
	{
		itoa(hours, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(hours, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ':';

	if (minutes >= 10)
	{
		itoa(minutes, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(minutes, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ':';

	if (seconds >= 10)
	{
		itoa(seconds, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(seconds, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ' ';
}


void Clock__getTimeWithSecondsString (char* buffer)
{
	Clock__convertTimeWithSecondsToString(Clock__getHours(), Clock__getMinutes(), Clock__getSeconds(), buffer);
}


void Clock__getDateString (char* buffer)
{
	uint8_t date;
	uint8_t idxBuffer = 0;

	date = Clock__getDate();

	if (date >= 10)
	{
		itoa(date, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(date, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ' ';
}


void Clock__getMonthString (char* buffer)
{
	uint8_t month;
	uint8_t idxBuffer = 0;

	month = Clock__getMonth();

	if (month >= 10)
	{
		itoa(month, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(month, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ' ';
}


void Clock__getYearString (char* buffer)
{
	uint8_t year;
	uint8_t idxBuffer = 0;

	year = Clock__getYear();

	if (year >= 10)
	{
		itoa(year, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(year, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ' ';
}


void Clock__convertDateToString (uint8_t date, uint8_t month, char *buffer)
{
	uint8_t idxBuffer = 0;

	if (date >= 10)
	{
		itoa(date, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(date, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = '/';

	if (month >= 10)
	{
		itoa(month, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(month, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ' ';
}


void Clock__getCompleteDateString (char* buffer)
{
	Clock__convertDateToString(Clock__getDate(), Clock__getMonth(), buffer);
}


void Clock__getCompleteDateWithYearString (char* buffer)
{
	uint8_t date, month, year;
	uint8_t idxBuffer = 0;

	date = Clock__getDate();
	month = Clock__getMonth();
	year = Clock__getYear();

	if (date >= 10)
	{
		itoa(date, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(date, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = '/';

	if (month >= 10)
	{
		itoa(month, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(month, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = '/';

	if (year >= 10)
	{
		itoa(year, &buffer[idxBuffer++], 10);
		idxBuffer++;
	}
	else
	{
		buffer[idxBuffer++] = '0';
		itoa(year, &buffer[idxBuffer++], 10);
	}

	buffer[idxBuffer++] = ' ';
}

#endif
