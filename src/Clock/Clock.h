/*
 * Clock.h
 *
 *  Created on: 12.03.2013
 *      Author: Jean-Martin George
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "Main_Types.h"
#include "Main_Config.h"
#include "Drivers.h"


typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} Clock_time;

#define CLOCK_SYNC_OK 		TRUE
#define CLOCK_SYNC_NOT_OK	FALSE

/* public functions */
void Clock__init (void);
void Clock__x10 (void);

uint8_t Clock__getSeconds (void);
uint8_t Clock__getMinutes (void);
uint8_t Clock__getHours (void);
uint8_t Clock__getDay (void);
uint8_t Clock__getDate (void);
uint8_t Clock__getMonth (void);
uint8_t Clock__getYear (void);


void Clock__incDate (void);
void Clock__incMonth (void);
void Clock__incYear (void);
void Clock__incHours (void);
void Clock__incMinutes (void);
void Clock__incSeconds (void);
void Clock__decDate (void);
void Clock__decMonth (void);
void Clock__decYear (void);
void Clock__decHours (void);
void Clock__decMinutes (void);
void Clock__decSeconds (void);

void Clock__getHourString  (char* buffer);
void Clock__getMinutesString  (char* buffer);
void Clock__getTimeString  (char* buffer);
void Clock__convertTimeWithSecondsToString (uint8_t hours, uint8_t minutes, uint8_t seconds, char* buffer);
void Clock__getTimeWithSecondsString  (char* buffer);
void Clock__getDateString  (char* buffer);
void Clock__getMonthString  (char* buffer);
void Clock__getYearString  (char* buffer);
void Clock__convertDateToString (uint8_t date, uint8_t month, char *buffer);
void Clock__getCompleteDateString (char* buffer);
void Clock__getCompleteDateWithYearString (char* buffer);

#endif /* CLOCK_H_ */
