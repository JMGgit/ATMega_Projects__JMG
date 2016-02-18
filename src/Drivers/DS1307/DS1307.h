/*
 * DS1307.h
 *
 *  Created on: 26.01.2013
 *      Author: Jean-Martin George
 */

#ifndef DS1307_H_
#define DS1307_H_

#include <avr/io.h>
#include <ATmega/uC.h>
#include "Main_Types.h"
#include "Drivers_Config.h"


#if (CLOCK_TYPE == CLOCK_TYPE_DS1307)

typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} DS1307_time;


/* public functions */
void DS1307__init (void);
void DS1307__sendTimeToRTC (void);
void DS1307__updateTimeFromRTC (void);

uint8_t DS1307__getSeconds (void);
uint8_t DS1307__getMinutes (void);
uint8_t DS1307__getHours (void);
uint8_t DS1307__getDay (void);
uint8_t DS1307__getDate (void);
uint8_t DS1307__getMonth (void);
uint8_t DS1307__getYear (void);

void DS1307__setSeconds (uint8_t seconds);
void DS1307__setMinutes (uint8_t minutes);
void DS1307__setHours (uint8_t hours);
void DS1307__setDay (uint8_t day);
void DS1307__setDate (uint8_t date);
void DS1307__setMonth (uint8_t month);
void DS1307__setYear (uint8_t year);


#define DS1307_ADDRESS		0x68

#endif

#endif /* DS1307_H_ */
