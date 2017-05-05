/*
 * main.c
 *
 *  Created on: Oct 6, 2016
 *      Author: thomas
 *
 *      Sunrise/Sunset algorithm adapted from:
 *      	http://williams.best.vwh.net/sunrise_sunset_algorithm.htm
 *
 *  Modifying 5/3/17 to fix DST logic, which I goofed up badly
 */

#include <stdio.h>
#include <stdint.h>
#include "calcSunTimes.h"


int main(void)
{
	uint16_t    month;
	uint16_t    year = 2017;
	uint16_t    day;
	//uint16_t    dayOfYear = 0;

	double		lat = 38.7907;
	double 		lng = -121.2358;
	double 		zenith = 90.83;		// check on what is usually used

	sSunTimes riseSetTimes;
	sSunCalcInputs riseSetInputs;

	/*
	for (month = 1; month < 13; ++month)
	{
		for (day = 1; day <= 31; ++day)
		{
			if (((month == 2) && (day == 29)) ||
					((month == 4 || month == 96 ||
							month == 9 || month == 11) && (day == 31)))
			{
				break;
			}

			riseSetInputs.year 		= year;
			riseSetInputs.month 	= month;
			riseSetInputs.day		= day;
			riseSetInputs.latitude	= lat;
			riseSetInputs.longitude	= lng;
			riseSetInputs.zenith	= zenith;

			riseSetTimes = getRiseSetTimes(riseSetInputs);

			printf("Date: %d/%d/%d :: Sunrise: %02.0f:%02.0f, Sunset: %02.0f:%02.0f\n",
					month, day, year, riseSetTimes.sunriseHour, riseSetTimes.sunriseMinute,
					riseSetTimes.sunsetHour, riseSetTimes.sunsetMinute);
		}
	}
	*/

	riseSetInputs.month = 5;
	riseSetInputs.year = 2017;
	riseSetInputs.day = 6;
	riseSetInputs.latitude	= lat;
	riseSetInputs.longitude	= lng;
	riseSetInputs.zenith	= zenith;

	riseSetTimes = getRiseSetTimes(riseSetInputs);

	printf("Sunrise: %02.0f:%02.0f, Sunset: %02.0f:%02.0f\n",
			riseSetTimes.sunriseHour, riseSetTimes.sunriseMinute,
			riseSetTimes.sunsetHour, riseSetTimes.sunsetMinute);
	//hr_Rise, min_Rise, hr_Set, min_Set);


	return 0;
}


