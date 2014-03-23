/*
 * Clock.h
 *
 *  Created on: 12.03.2013
 *      Author: Jean-Martin George
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "../Main/Main_Types.h"
#include "../Main/Main_Config.h"
#include "../Drivers/Drivers.h"


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

void Clock__incHours (void);
void Clock__incMinutes (void);

#endif /* CLOCK_H_ */
