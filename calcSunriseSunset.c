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
 *  o Modified to use local TZ for offset in calcSunTimes.c
 *  o Pull lat, long, zenith from a config file (latlong.dat)
 *  o 5/10/17 Cleaning up comments and moving toward version
 *    that will add or subtract optime offsets
 */

#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
//#include <stdint.h>
#include <string.h>
#include <time.h>
#include "calcSunTimes.h"

#define TWILIGHT_TIME 45.0	/**< Time before sunset and after sunrise to toggle operation */

uint8_t getLatLong(sSunCalcInputs *riseSetInputs);
uint8_t readOpTimesDat(uint8_t *opTimes);
uint8_t writeOpTimesDat(uint8_t *opTimes);

int main(void)
{
	sSunTimes riseSetTimes;
	sSunCalcInputs riseSetInputs;

	uint8_t opTimes[5] = {0};
	//uint8_t i;

	getLatLong(&riseSetInputs);
	readOpTimesDat(opTimes);	// Test values: 17, 32, 8, 15, 1
//	for (i = 0; i < 5; ++i)
//	{
//		printf("opTimes[%d] = %d\n", i, opTimes[i]);
//	}

	time_t now = time(NULL);
	struct tm *currTime = localtime(&now);

	riseSetInputs.month = currTime->tm_mon + 1;
	riseSetInputs.year = currTime->tm_year + 1900;
	riseSetInputs.day = currTime->tm_mday;

	riseSetTimes = getRiseSetTimes(riseSetInputs);
	//printf("Lat = %lf\n", riseSetInputs.latitude);

	// change times by given amount (#define TWILIGHT_TIME XX)
	struct tm tm;
	char buf[255];

	// Modify sunset time
	memset(&tm, 0, sizeof(struct tm));
	snprintf(buf, sizeof(buf), "%d-%d-%d %d:%d:%d",
			(int) riseSetInputs.year, (int) riseSetInputs.month,
			(int) riseSetInputs.day, (int) riseSetTimes.sunsetHour,
			(int) riseSetTimes.sunsetMinute, (int) 0);
	printf("%s\n", buf);
	strptime(buf, "%Y-%m-%d %H:%M:%S", &tm);
	printf("strptime buffer is %s\n", asctime(&tm));

	printf("tm.hour before subtract %d\n", tm.tm_hour);
	tm.tm_min -= TWILIGHT_TIME;
	printf("tm.hour after subtract %d\n", tm.tm_hour);
	printf("tm.hour before mktime(): %d\n", tm.tm_hour);
	printf("tm.min before mktime(): %d\n", tm.tm_min);
	timegm(&tm);
	printf("tm.hour after mktime(): %d\n", tm.tm_hour);
	printf("tm.min after mktime(): %d\n", tm.tm_min);
	printf("%.0f min before sunset would be: %s\n", TWILIGHT_TIME, asctime(&tm));
	opTimes[0] = tm.tm_hour;
	opTimes[1] = tm.tm_min;

	// Modify sunrise time
	memset(&tm, 0, sizeof(struct tm));
	snprintf(buf, sizeof(buf), "%d-%d-%d %d:%d:%d",
			(int) riseSetInputs.year, (int) riseSetInputs.month,
			(int) riseSetInputs.day, (int) riseSetTimes.sunriseHour,
			(int) riseSetTimes.sunriseMinute, (int) 0);
	printf("%s\n", buf);
	strptime(buf, "%Y-%m-%d %H:%M:%S", &tm);
	printf("strptime buffer is %s\n", asctime(&tm));

	printf("tm.hour before add: %d\n", tm.tm_hour);
	tm.tm_min += TWILIGHT_TIME;
	printf("tm.hour after add: %d\n", tm.tm_hour);
	printf("tm.hour before mktime(): %d\n", tm.tm_hour);
	printf("tm.min before mktime(): %d\n", tm.tm_min);
	timegm(&tm);
	printf("tm.hour after mktime(): %d\n", tm.tm_hour);
	printf("tm.min after mktime(): %d\n", tm.tm_min);
	printf("%.0f min after sunrise would be: %s\n", TWILIGHT_TIME, asctime(&tm));
	opTimes[2] = tm.tm_hour;
	opTimes[3] = tm.tm_min;

	// Write new values out to file
	uint8_t i;
	for (i = 0; i < 5; ++i)
	{
		printf("opTimes[%d] = %d\n", i, opTimes[i]);
	}
	writeOpTimesDat(opTimes);

	printf("For %d/%d/%d:: Sunrise: %02.0f:%02.0f, Sunset: %02.0f:%02.0f\n",
				riseSetInputs.month, riseSetInputs.day, riseSetInputs.year,
				riseSetTimes.sunriseHour, riseSetTimes.sunriseMinute,
				riseSetTimes.sunsetHour, riseSetTimes.sunsetMinute);

	return 0;
}

uint8_t getLatLong(sSunCalcInputs *riseSetInputs)
{
	FILE *fd;
	char *configFile = "/home/sil/bin/latlong.dat";

	fd = fopen(configFile, "r");
	if (NULL != fd)
	{
		fscanf(fd, "%lf, %lf, %lf\n",
				&riseSetInputs->latitude, &riseSetInputs->longitude, &riseSetInputs->zenith);
		fclose(fd);
		return 0;
	}
	else
	{
		perror("Could not open latlong.dat file to get lat/long/zenith...\n");
		return -1;
	}
}

uint8_t readOpTimesDat(uint8_t *opTimes)
{
	// read in data
	FILE *fd;
	char *configFile = "/home/sil/bin/optimes.dat";

	fd = fopen(configFile, "r");
	if (NULL != fd)
	{
		fscanf(fd, "%" SCNd8 ", %" SCNd8 ", %" SCNd8 ", %" SCNd8 ", %" SCNd8 "\n",
				&opTimes[0], &opTimes[1], &opTimes[2], &opTimes[3], &opTimes[4]);
		fclose(fd);
		return 0;
	}
	else
	{
		perror("Could not open optimes.dat file to get operation times...\n");
		return -1;
	}

	return 0;
}

uint8_t writeOpTimesDat(uint8_t *opTimes)
{
	// Write the modified file here
	FILE *fd;
	char *configFile = "/home/sil/bin/optimes.dat";

	fd = fopen(configFile, "w");
	if (NULL != fd)
	{
		fprintf(fd, "%" SCNd8 ", %" SCNd8 ", %" SCNd8 ", %" SCNd8 ", %" SCNd8 "\n",
				opTimes[0], opTimes[1], opTimes[2], opTimes[3], opTimes[4]);
		fclose(fd);
		return 0;
	}
	else
	{
		perror("Could not open optimes.dat file to set operation times...\n");
		return -1;
	}


	return 0;
}
