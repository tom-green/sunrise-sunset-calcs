/*
 * calcSunTimes.h
 *
 *  Created on: May 4, 2017
 *      Author: thomas
 */

#ifndef CALCSUNTIMES_H_
#define CALCSUNTIMES_H_

typedef struct {
	double sunriseHour;
	double sunriseMinute;
	double sunsetHour;
	double sunsetMinute;
} sSunTimes;

typedef struct {
	double latitude;
	double longitude;
	double zenith;
	uint8_t day;
	uint8_t month;
	uint16_t year;
} sSunCalcInputs;

sSunTimes getRiseSetTimes(sSunCalcInputs sRiseSetInputs);

#endif /* CALCSUNTIMES_H_ */
