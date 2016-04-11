/*
 * DCF77.h
 *
 *  Created on: 03.03.2013
 *      Author: Jean-Martin George
 */

#ifndef DCF77_H_
#define DCF77_H_

#include <avr/io.h>
#include "Main_Types.h"
#include "Main_Config.h"
#include "Drivers_Config.h"
#include "uC.h"


#if (CLOCK_SYNC == CLOCK_SYNC_DCF77)

typedef struct
{
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} DCF77_time;


#define DCF77_INDEX_MINUTES_1		21
#define DCF77_INDEX_MINUTES_2		22
#define DCF77_INDEX_MINUTES_4		23
#define DCF77_INDEX_MINUTES_8		24
#define DCF77_INDEX_MINUTES_10		25
#define DCF77_INDEX_MINUTES_20		26
#define DCF77_INDEX_MINUTES_40		27
#define DCF77_INDEX_MINUTES_CHECK	28

#define DCF77_INDEX_HOURS_1			29
#define DCF77_INDEX_HOURS_2			30
#define DCF77_INDEX_HOURS_4			31
#define DCF77_INDEX_HOURS_8			32
#define DCF77_INDEX_HOURS_10		33
#define DCF77_INDEX_HOURS_20		34
#define DCF77_INDEX_HOURS_CHECK		35

#define DCF77_INDEX_DAY_1			42
#define DCF77_INDEX_DAY_2			43
#define DCF77_INDEX_DAY_4			44

#define DCF77_INDEX_DATE_1			36
#define DCF77_INDEX_DATE_2			37
#define DCF77_INDEX_DATE_4			38
#define DCF77_INDEX_DATE_8			39
#define DCF77_INDEX_DATE_10			40
#define DCF77_INDEX_DATE_20			41

#define DCF77_INDEX_MONTH_1			45
#define DCF77_INDEX_MONTH_2			46
#define DCF77_INDEX_MONTH_4			47
#define DCF77_INDEX_MONTH_8			48
#define DCF77_INDEX_MONTH_10		49

#define DCF77_INDEX_YEAR_1			50
#define DCF77_INDEX_YEAR_2			51
#define DCF77_INDEX_YEAR_4			52
#define DCF77_INDEX_YEAR_8			53
#define DCF77_INDEX_YEAR_10			54
#define DCF77_INDEX_YEAR_20			55
#define DCF77_INDEX_YEAR_40			56
#define DCF77_INDEX_YEAR_80			57

#define DCF77_INDEX_DATE_CHECK		58

#define DCF77_MIN_TIME_BIT_1		((150 * 10) / DCF77_TASK_TIME)
#define DCF77_MAX_TIME_BIT_1		((250  * 10) / DCF77_TASK_TIME)
#define DCF77_MIN_TIME_BIT_0		((50  * 10) / DCF77_TASK_TIME)
#define DCF77_MAX_TIME_BIT_0		((150  * 10) / DCF77_TASK_TIME)
#define DCF77_MIN_TIME_BIT_PAUSE	(((1000  * 10) / DCF77_TASK_TIME) - DCF77_MAX_TIME_BIT_1)
#define DCF77_MAX_TIME_BIT_PAUSE	(((1000  * 10) / DCF77_TASK_TIME)- DCF77_MIN_TIME_BIT_0)
#define DCF77_MIN_TIME_SYNC_PAUSE	((1500  * 10) / DCF77_TASK_TIME)
#define DCF77_MAX_TIME_SYNC_PAUSE	((2500  * 10) / DCF77_TASK_TIME)
#define DCF77_TIME_OUT				((2500  * 10) / DCF77_TASK_TIME)
#define DCF77_SAMPLES				59

#define DCF77_STATUS_OK				0
#define DCF77_STATUS_PENDING		1
#define DCF77_STATUS_FAILED			2


/* public functions */
void DCF77__init (void);

uint8_t DCF77__getMinutes (void);
uint8_t DCF77__getHours (void);
uint8_t DCF77__getDay (void);
uint8_t DCF77__getDate (void);
uint8_t DCF77__getMonth (void);
uint8_t DCF77__getYear (void);
uint8_t DCF77__updateSignal (void);

#endif

#endif /* DCF77_H_ */
