/*
 * DCF77.c
 *
 *  Created on: 03.03.2013
 *      Author: Jean-Martin George
 */

#include "DCF77.h"
#include "../../Modes/Modes.h"

#if (CLOCK_SYNC == CLOCK_SYNC_DCF77)


static uint8_t DCF77_signal[DCF77_SAMPLES];
static DCF77_time time;


void DCF77__init (void)
{
	setInput(DCF77_DATA_DDR, DCF77_DATA_PIN);
	setLow(DCF77_DATA_PORT, DCF77_DATA_PIN);
	setOutput(DCF77_LED_DDR, DCF77_LED_PIN);
}


uint8_t DCF77__updateSignal (void)
{
	static uint16_t bitCycles = 0;
	static uint8_t bitState = 0;
	static uint8_t itSample = 0;
	static uint8_t dataCorrupt = TRUE;
	uint8_t retVal = DCF77_STATUS_FAILED;

	if (isHigh(DCF77_DATA_PORT, DCF77_DATA_PIN))
	{
		setHigh(DCF77_LED_PORT, DCF77_LED_PIN);

		if (bitState == 0)
		{
			/* first cycle with state high
			 * 1. evaluate time since last pulse (state low)
			 * 2. reset counter and state
			 */

			/* 1. */
			if (	(bitCycles > DCF77_MIN_TIME_SYNC_PAUSE)
				&&	(bitCycles < DCF77_MAX_TIME_SYNC_PAUSE)
				)
			{
				/* PAUSE --> start sampling */
				itSample = 0;
				dataCorrupt = FALSE;
			}
			else if (	(bitCycles > DCF77_MIN_TIME_BIT_PAUSE)
					&&	(bitCycles < DCF77_MAX_TIME_BIT_PAUSE)
					)
			{
				/* correct low state */
			}
			else
			{
				/* state not defined */
				dataCorrupt = TRUE;
			}

			/* 2. */
			bitCycles = 0;
		}
		else
		{
			/* count the cycles in state high */
			bitCycles++;

			if (bitCycles > DCF77_TIME_OUT)
			{
				dataCorrupt = TRUE;
			}
		}

		/* set state to high */
		bitState = 1;
	}
	else
	{
		setLow(DCF77_LED_PORT, DCF77_LED_PIN);

		if (bitState == 1)
		{
			/* first cycle with state low
			 * 1. evaluate time of last pulse (state high)
			 * 2. reset counter, set next sample
			 */

			/* 1. */
			if (	(bitCycles > DCF77_MIN_TIME_BIT_1)
				&&	(bitCycles < DCF77_MAX_TIME_BIT_1)
				)
			{
				/* bit 1 */
				DCF77_signal[itSample] = 1;
				itSample++;
			}
			else if (	(bitCycles > DCF77_MIN_TIME_BIT_0)
					&&	(bitCycles < DCF77_MAX_TIME_BIT_0)
					)
			{
				/* bit 0 */
				DCF77_signal[itSample] = 0;
				itSample++;
			}
			else
			{
				/* state not defined */
				dataCorrupt = TRUE;
			}

			/* 2. */
			bitCycles = 0;
		}
		else
		{
			/* count the cycles in state low */
			bitCycles++;

			if (bitCycles > DCF77_TIME_OUT)
			{
				dataCorrupt = TRUE;
			}
		}

		/* set state to low */
		bitState = 0;
	}


	if (dataCorrupt == TRUE)
	{
		itSample = 0;
		retVal = DCF77_STATUS_FAILED;
	}
	else if (itSample < DCF77_SAMPLES)
	{
		retVal = DCF77_STATUS_PENDING;
	}
	else
	{
		itSample = 0;
		/* check parity bits */
		if ((((	DCF77_signal[DCF77_INDEX_MINUTES_1]
		      + DCF77_signal[DCF77_INDEX_MINUTES_2]
		      + DCF77_signal[DCF77_INDEX_MINUTES_4]
		      + DCF77_signal[DCF77_INDEX_MINUTES_8]
		      + DCF77_signal[DCF77_INDEX_MINUTES_10]
		      + DCF77_signal[DCF77_INDEX_MINUTES_20]
		      + DCF77_signal[DCF77_INDEX_MINUTES_40]) % 2)
			!= (DCF77_signal[DCF77_INDEX_MINUTES_CHECK]))
		|| (((	DCF77_signal[DCF77_INDEX_HOURS_1]
		      + DCF77_signal[DCF77_INDEX_HOURS_2]
		      + DCF77_signal[DCF77_INDEX_HOURS_4]
		      + DCF77_signal[DCF77_INDEX_HOURS_8]
		      + DCF77_signal[DCF77_INDEX_HOURS_10]
		      + DCF77_signal[DCF77_INDEX_HOURS_20]) % 2)
			!= (DCF77_signal[DCF77_INDEX_HOURS_CHECK]))
		|| (((	DCF77_signal[DCF77_INDEX_DAY_1]
		      + DCF77_signal[DCF77_INDEX_DAY_2]
		      + DCF77_signal[DCF77_INDEX_DAY_4]
		      + DCF77_signal[DCF77_INDEX_DATE_1]
		      + DCF77_signal[DCF77_INDEX_DATE_2]
		      + DCF77_signal[DCF77_INDEX_DATE_4]
		      + DCF77_signal[DCF77_INDEX_DATE_8]
		      + DCF77_signal[DCF77_INDEX_DATE_10]
		      + DCF77_signal[DCF77_INDEX_DATE_20]
  		      + DCF77_signal[DCF77_INDEX_MONTH_1]
   		      + DCF77_signal[DCF77_INDEX_MONTH_2]
   		      + DCF77_signal[DCF77_INDEX_MONTH_4]
  		      + DCF77_signal[DCF77_INDEX_MONTH_8]
   		      + DCF77_signal[DCF77_INDEX_MONTH_10]
   		      + DCF77_signal[DCF77_INDEX_YEAR_1]
   		      + DCF77_signal[DCF77_INDEX_YEAR_2]
   		      + DCF77_signal[DCF77_INDEX_YEAR_4]
   		      + DCF77_signal[DCF77_INDEX_YEAR_8]
  		      + DCF77_signal[DCF77_INDEX_YEAR_10]
   		      + DCF77_signal[DCF77_INDEX_YEAR_20]
   		      + DCF77_signal[DCF77_INDEX_YEAR_40]
   		      + DCF77_signal[DCF77_INDEX_YEAR_80]) % 2)
			!= (DCF77_signal[DCF77_INDEX_DATE_CHECK]))
		)
		{
			dataCorrupt = TRUE;
			retVal = DCF77_STATUS_FAILED;
		}
		else
		{
			/* check OK --> copy data */
			time.minutes = (DCF77_signal[DCF77_INDEX_MINUTES_1] * 1)
						 + (DCF77_signal[DCF77_INDEX_MINUTES_2] * 2)
						 + (DCF77_signal[DCF77_INDEX_MINUTES_4] * 4)
						 + (DCF77_signal[DCF77_INDEX_MINUTES_8] * 8)
						 + (DCF77_signal[DCF77_INDEX_MINUTES_10] * 10)
						 + (DCF77_signal[DCF77_INDEX_MINUTES_20] * 20)
						 + (DCF77_signal[DCF77_INDEX_MINUTES_40] * 40);

			time.hours   = (DCF77_signal[DCF77_INDEX_HOURS_1] * 1)
						 + (DCF77_signal[DCF77_INDEX_HOURS_2] * 2)
						 + (DCF77_signal[DCF77_INDEX_HOURS_4] * 4)
						 + (DCF77_signal[DCF77_INDEX_HOURS_8] * 8)
						 + (DCF77_signal[DCF77_INDEX_HOURS_10] * 10)
						 + (DCF77_signal[DCF77_INDEX_HOURS_20] * 20);

			time.day 	 = (DCF77_signal[DCF77_INDEX_DAY_1] * 1)
						 + (DCF77_signal[DCF77_INDEX_DAY_2] * 2)
						 + (DCF77_signal[DCF77_INDEX_DAY_4] * 4);

			time.date    = (DCF77_signal[DCF77_INDEX_DATE_1] * 1)
						 + (DCF77_signal[DCF77_INDEX_DATE_2] * 2)
						 + (DCF77_signal[DCF77_INDEX_DATE_4] * 4)
						 + (DCF77_signal[DCF77_INDEX_DATE_8] * 8)
						 + (DCF77_signal[DCF77_INDEX_DATE_10] * 10)
						 + (DCF77_signal[DCF77_INDEX_DATE_20] * 20);

			time.month   = (DCF77_signal[DCF77_INDEX_MONTH_1] * 1)
						 + (DCF77_signal[DCF77_INDEX_MONTH_2] * 2)
						 + (DCF77_signal[DCF77_INDEX_MONTH_4] * 4)
						 + (DCF77_signal[DCF77_INDEX_MONTH_8] * 8)
						 + (DCF77_signal[DCF77_INDEX_MONTH_10] * 10);

			time.year    = (DCF77_signal[DCF77_INDEX_YEAR_1] * 1)
						 + (DCF77_signal[DCF77_INDEX_YEAR_2] * 2)
						 + (DCF77_signal[DCF77_INDEX_YEAR_4] * 4)
						 + (DCF77_signal[DCF77_INDEX_YEAR_8] * 8)
						 + (DCF77_signal[DCF77_INDEX_YEAR_10] * 10)
						 + (DCF77_signal[DCF77_INDEX_YEAR_20] * 20)
						 + (DCF77_signal[DCF77_INDEX_YEAR_40] * 40)
						 + (DCF77_signal[DCF77_INDEX_YEAR_80] * 80);

			retVal = DCF77_STATUS_OK;
		}
	}

	return retVal;
}


uint8_t DCF77__getMinutes (void)		{return time.minutes;}
uint8_t DCF77__getHours (void)			{return time.hours;}
uint8_t DCF77__getDay (void)			{return time.day;}
uint8_t DCF77__getDate (void)			{return time.date;}
uint8_t DCF77__getMonth (void)			{return time.month;}
uint8_t DCF77__getYear (void)			{return time.year;}
#endif
