/*
 * calcSunTimes.c
 *
 *  Created on: May 4, 2017
 *      Author: thomas
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "calcSunTimes.h"

// This prototype not necessary, but for the sake of following standard practice:
static uint16_t getDayOfYear(uint16_t day, uint16_t month, uint16_t year);

static uint16_t getDayOfYear(uint16_t day, uint16_t month, uint16_t year)
{
	int16_t    N1 = floor(275 * month / 9);
	int16_t    N2 = floor((month + 9) / 12);
	int16_t    N3 = (1 + floor((year - 4 * floor(year / 4) + 2) / 3));
	return N1 - (N2 * N3) + day - 30;
}

sSunTimes getRiseSetTimes(sSunCalcInputs sRiseSetInputs)
	{
	sSunTimes sRiseSetTimes;
	uint16_t 	N 		= 0;
	double		tRise 	= 0.0;
	double		tSet 	= 0.0;
	double 		offset 	= 7.0;

	N = getDayOfYear(sRiseSetInputs.day, sRiseSetInputs.month, sRiseSetInputs.year);

	// Convert the longitude to hour value and calculate an approximate time
	double lngHour = sRiseSetInputs.longitude / 15.0;
	tRise = N + ((6 - lngHour) / 24);
	tSet = N + ((18 - lngHour) / 24);

	// Calculate the sun's mean anomaly
	double mRise = ((0.9856 * tRise) - 3.289);
	double mSet = ((0.9856 * tSet) - 3.289);

	// Calculate the sun's true longitude
	double L_Rise = fmod(mRise + (1.916 * sin((M_PI/180.0)*mRise)) + (0.020 * sin(2 * (M_PI/180.0)*mRise)) + 282.634, 360.0);
	double L_Set = fmod(mSet + (1.916 * sin((M_PI/180.0)*mSet)) + (0.020 * sin(2 * (M_PI/180.0)*mSet)) + 282.634, 360.0);

	// Calculate the sun's right ascension
	// TODO Conditionals
	double raRise = fmod((180.0/M_PI)*atan(0.91764 * tan((M_PI/180.0)*L_Rise)), 360.0);
	double raSet = fmod((180.0/M_PI)*atan(0.91764 * tan((M_PI/180.0)*L_Set)), 360.0);

	// Right ascension value needs to be in the same quadrant as true longitude
	double Lquad_Rise = (floor(L_Rise / 90)) * 90;
	double Lquad_Set = (floor(L_Set / 90)) * 90;
	double raQuadRise = (floor(raRise / 90)) * 90;
	double raQuadSet = (floor(raSet / 90)) * 90;
	raRise = raRise + (Lquad_Rise - raQuadRise);
	raSet = raSet + (Lquad_Set - raQuadSet);

	// Right ascension value needs to be converted into hours
	raRise = raRise / 15.0;
	raSet = raSet / 15.0;

	// Calculate the sun's declination
	double sinDec_Rise = 0.39782 * sin((M_PI/180.0)*L_Rise);
	double cosDec_Rise = cos(asin(sinDec_Rise));

	double sinDec_Set = 0.39782 * sin((M_PI/180.0)*L_Set);
	double cosDec_Set = cos(asin(sinDec_Set));

	// Calculate the sun's local hour angle
	// TODO Conditionals
	double cosH_Rise = (cos((M_PI/180.0)*sRiseSetInputs.zenith) -
			(sinDec_Rise * sin((M_PI/180.0)*sRiseSetInputs.latitude))) /
					(cosDec_Rise * cos((M_PI/180.0)*sRiseSetInputs.latitude));
	double cosH_Set = (cos((M_PI/180.0)*sRiseSetInputs.zenith) -
			(sinDec_Set * sin((M_PI/180.0)*sRiseSetInputs.latitude))) /
					(cosDec_Set * cos((M_PI/180.0)*sRiseSetInputs.latitude));

	// Finish calculating H and convert into hours
	double hRise = 360 - (180.0/M_PI)*acos(cosH_Rise); //((M_PI/180.0)*cosH_Rise);
	double hSet = (180.0/M_PI)*acos(cosH_Set); //((M_PI/180.0)*cosH_Set);
	hRise = hRise / 15.0;
	hSet = hSet / 15.0;

	// Calculate local mean time of rising/setting
	double loc_tRise = hRise + raRise - (0.06571 * tRise) - 6.622;
	double loc_tSet = hSet + raSet - (0.06571 * tSet) - 6.622;

	// Adjust back to UTC
	// TODO Conditional
	double utcRise = fmod(loc_tRise - lngHour, 24.0);
	double utcSet = fmod(loc_tSet - lngHour, 24.0);

	// Convert UTC value to local time zone of lat/long
	if (((sRiseSetInputs.month > 3) && (sRiseSetInputs.month < 11)) ||
			((sRiseSetInputs.month == 3) && (sRiseSetInputs.day > 12)) ||
			((sRiseSetInputs.month == 11) && (sRiseSetInputs.day < 5)))
	{
		offset = 7.0;
	}
	else
	{
		offset = 8.0;
	}
	double localT_Rise = utcRise - offset;
	double localT_Set = utcSet - offset;
	if (localT_Set < 0)
	{
		localT_Set += 24.0;
	}

	//double hr_Rise;
	//double hr_Set;
	sRiseSetTimes.sunriseMinute = floor(modf(localT_Rise, &sRiseSetTimes.sunriseHour) * 60);
	sRiseSetTimes.sunsetMinute = floor(modf(localT_Set, &sRiseSetTimes.sunsetHour) * 60);
	//printf("Sunrise: %02.0f:%02.0f, Sunset: %02.0f:%02.0f\n", hr_Rise, min_Rise, hr_Set, min_Set);


	//printf("Date: %d/%d/%d, day of year = %d :: Sunrise: %02.0f:%02.0f, Sunset: %02.0f:%02.0f\n", month, day, year, N, hr_Rise, min_Rise, hr_Set, min_Set);

	return sRiseSetTimes;
}
