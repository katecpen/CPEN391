#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "graphics.h"
#include "Colours.h"
#include "Gps.h"
#include "map.h"
#include "previoussessions.h"

// Hard-coded mapping of latitude-longitude points to the screen's x, y coordinates
float latitude_test[20];
float longitude_test[20];

/*
 * Initialize the mapping of latitude-longitude points to the screen's x, y coordinates
 */
void initPointsTest(void) {
	latitude_test[0] = 49.262105;
	longitude_test[0] = -123.250211;

	latitude_test[1] = 49.261995;
	longitude_test[1] = -123.250240;

	latitude_test[2] = 49.261887;
	longitude_test[2] = -123.250170;

	latitude_test[3] = 49.261766;
	longitude_test[3] = -123.250081;

	latitude_test[4] = 49.261622;
	longitude_test[4] = -123.250038;

	latitude_test[5] = 49.261521;
	longitude_test[5] = -123.249773;

	latitude_test[6] = 49.261526;
	longitude_test[6] = -123.249601;

	latitude_test[7] = 49.261596;
	longitude_test[7] = -123.249333;

	latitude_test[8] = 49.261706;
	longitude_test[8] = -123.249118;

	latitude_test[9] = 49.261787;
	longitude_test[9] = -123.248904;

	latitude_test[10] = 49.261902;
	longitude_test[10] = -123.248807;

	latitude_test[11] = 49.261999;
	longitude_test[11] = -123.248622;

	latitude_test[12] = 49.262081;
	longitude_test[12] = -123.248424;

	latitude_test[13] = 49.262098;
	longitude_test[13] = -123.248603;

	latitude_test[14] = 49.262049;
	longitude_test[14] = -123.248936;

	latitude_test[15] = 49.262002;
	longitude_test[15] = -123.249153;

	latitude_test[16] = 49.262076;
	longitude_test[16] = -123.249505;

	latitude_test[17] = 49.262048;
	longitude_test[17] = -123.249778;

	latitude_test[18] = 49.261978;
	longitude_test[18] = -123.250001;

	latitude_test[19] = 49.262105;
	longitude_test[19] = -123.250211;
}

/*
 * Initialize the GPS
 */
void Init_GPS(void) {
	// Set up 6850 Control Register to utilize a divide by 16 clock,
	// use 8 bits of data, no parity, 1 stop bit,
	// Program baud rate generator to use 9600 baud
	GPS_Control = 0x15;
	GPS_Baud = 0x05;
	//printf("Initialized GPS\n");
}

/*
 * Poll and return the received character from the GPS
 */
char getDataGPS(void) {
	// Poll Rx bit in 6850 status register. Wait for it to become '1'.
	while ((GPS_Status & 1) == 0) {
		// Wait for it to become '1'
	}

	// Read received character from 6850 RxData register.
	return GPS_RxData;
}

/*
 * Store the time extracted from the GPS into current_time
 */
void getTime(char *current_time) {
	char c = getDataGPS();

	// If c is a comma, then the time field of the $GPGGA log is empty
	if (c != ',') {
		char hours_utc[3]; // Store hh with null character
		char minutes[3]; // Store mm with null character
		char seconds[7]; // Store ss.sss with null character

		// Extract hh into hours_utc and convert hours from UTC to PST
		hours_utc[0] = c;
		hours_utc[1] = getDataGPS();
		hours_utc[2] = '\0';
		int hours_pst = atoi(hours_utc);
		if (hours_pst - 8 < 0) {
			hours_pst += 16;
		} else {
			hours_pst -= 8;
		}

		// Extract mm into minutes
		int i;
		for (i = 0; i < 2; i++) {
			minutes[i] = getDataGPS();
		}
		minutes[i] = '\0';
		int minutes_int = atoi(minutes);

		// Extract ss.sss into seconds
		for (i = 0; i < 6; i++) {
			seconds[i] = getDataGPS();
		}
		seconds[i] = '\0';
		int seconds_int = (int)(atof(seconds));

		// Calculate the current time in seconds
		time_seconds = hours_pst * 3600 + minutes_int * 60 + seconds_int;

		sprintf(current_time, "%d:%d:%d PST", hours_pst, minutes_int, seconds_int);

		// Extract comma
		getDataGPS();
	} else {
		sprintf(current_time, "Invalid/Empty");
	}

	// If first log of the session (or before session starts):
	// Set starting time as the current time
	if (extracted_first_log == 0) {
		start_time_seconds = time_seconds;
		sprintf(start_time, "%s", current_time);
	}
}

/*
 * Store the total time elapsed since the start of the session into time_elapsed
 */
void getTimeElapsed(char *time_elapsed) {
	time_elapsed_seconds = time_seconds - start_time_seconds;
	int hours_elapsed = time_elapsed_seconds / 3600;
	int seconds_elapsed = time_elapsed_seconds % 3600;
	int minutes_elapsed = time_elapsed_seconds / 60;
	seconds_elapsed = seconds_elapsed % 60;
	sprintf(time_elapsed, "%d:%d:%d ", hours_elapsed, minutes_elapsed, seconds_elapsed);
}

/*
 * Store the current field of the $GPGGA log into field
 */
void getField(char *field) {
	char c = getDataGPS();

	// If c is a comma, then the current field of the $GPGGA log is empty
	if (c != ',') {
		char temp[DATASIZE]; // Store contents of the field into temp

		int i = 0;

		// Store each character of the field into temp (until the next comma)
		while ((c != ',')) {
			temp[i] = c;
			c = getDataGPS();
			i++;
		}
		temp[i] = '\0';
		sprintf(field, &temp[0]);
	} else {
		sprintf(field, "Invalid/Empty");
	}
}

/*
 * Get hard-coded latitude for testing and demo purposes
 */
float getTestLatitude() {
	return latitude_test[test_index];
}

/*
 * Store the latitude extracted from the GPS in latitude (in signed degree format)
 */
void getLatitude(char *latitude) {

	char c = getDataGPS();

	// If c is a comma, then the latitude field of the $GPGGA log is empty
	if (c != ',') {
		char degrees[3]; // Store dd with null character
		char minutes[8]; // Store mm.mmmm with null character

		// Extract dd into degrees
		degrees[0] = c;
		degrees[1] = getDataGPS();
		degrees[2] = '\0';

		// Extract mm.mmmm into minutes
		int i;
		for (i = 0; i < 7; i++) {
			minutes[i] = getDataGPS();
		}
		minutes[i] = '\0';

		// Calculate latitude = dd + mm.mmmm / 60
		float degrees_float = atof(degrees);
		float minutes_float = atof(minutes);
		latitude_float = degrees_float + minutes_float / 60;

		// Extract comma
		getDataGPS();

		// Extract the direction unit (N/S) of the latitude
		char *direction;
		getField(direction);
		if (strcmp(direction, "S") == 0) {
			latitude_float = 0 - latitude_float;
		}

		sprintf(latitude, "%f", latitude_float);
	} else {
		sprintf(latitude, "Invalid/Empty");
	}

	// If first log of the session (or before session starts):
	// Set starting latitude as the current latitude
	latitude_float = getTestLatitude(); // Get hard-coded latitude for testing and demo purposes, uncomment when using in real-time

	if (extracted_first_log == 0) {
		start_latitude_float = latitude_float;
		previous_latitude_float = latitude_float;
		sprintf(start_latitude, "%s", latitude);
	}
}

/*
 * Get hard-coded longitude for testing and demo purposes
 */
float getTestLongitude(void) {
	return longitude_test[test_index];
}

/*
 * Store the longitude extracted from the GPS in longitude (in signed degree format)
 */
void getLongitude(char *longitude) {

	char c = getDataGPS();
	// If c is a comma, then the longitude field of the $GPGGA log is empty
	if (c != ',') {
		char degrees[4];
		char minutes[8];

		// Extract dd
		degrees[0] = c;
		int i;
		for (i = 1; i < 3; i++) {
			degrees[i] = getDataGPS();
		}
		degrees[i] = '\0';

		// Extract mmm.mmmm
		for (i = 0; i < 7; i++) {
			minutes[i] = getDataGPS();
		}
		minutes[i] = '\0';

		// Calculate longitude = dd + mmm.mmmm / 60
		float degrees_float = atof(degrees);
		float minutes_float = atof(minutes);
		longitude_float = degrees_float + minutes_float / 60;

		// Extract comma
		getDataGPS();

		// Extract the direction unit (N/S) of the latitude
		char *direction;
		getField(direction);
		if (strcmp(direction, "W") == 0) {
			longitude_float = 0 - longitude_float;
		}

		sprintf(longitude, "%f", longitude_float);
	} else {
		sprintf(longitude, "Invalid/Empty");
	}

	// If first log of the session (or before session starts):
	// Set starting longitude as the current longitude
	longitude_float = getTestLongitude(); // Get hard-coded longitude for testing and demo purposes, uncomment when using in real-time
	if (extracted_first_log == 0) {
		start_longitude_float = longitude_float;
		previous_longitude_float = longitude_float;
		sprintf(start_longitude, "%s", longitude);
	}
}

/*
 * Return the degree in radians
 */
double deg2rad(double degree) {
	return degree * pi / 180;
}

/*
 * Return the radians in degrees
 */
double rad2deg(double rad) {
	return rad * 180 / pi;
}

/*
 * Store the distance in meters between the last two logged points into distance
 * Store the speed in m/s between these two points into speed
 */
void getDistanceAndSpeed(char *distance, char *speed) {

	// If session has not started, distance and speed are both 0
	if (session_started == 0) {
		distance_int = 0;
		speed_int = 0;
		strcpy(distance, "0");
		strcpy(speed, "0");
		return;
	}

	// Calculate the distance
	// Adapted from http://www.geodatasource.com/developers/c
	double theta = previous_longitude_float - longitude_float;
	double distance_float = sin(deg2rad(((double)previous_latitude_float)));
	distance_float *= sin(deg2rad((double)latitude_float));
	distance_float += cos(deg2rad((double)previous_latitude_float)) * cos(deg2rad((double)latitude_float)) * cos(deg2rad(theta));
	distance_float = acos(distance_float);
	distance_float = rad2deg(distance_float);
	distance_float = distance_float * 60 * 1.1515;
	distance_float = distance_float * 1.609344 * 1000;

	// Since distance is in meters and speed is in m/s, and since logs are extracted at 1 second intervals,
	// speed (m/s) = distance (m)
	speed_int = (int)distance_float;
	sprintf(speed, "%d", speed_int);

	// If first log of the session (or before session starts):
	// Set total distance to distance between last two points
	// Set maximum speed to speed between last two points (speed in m/s = distance in m)
	if (extracted_first_log == 0) {
		distance_int = (int)distance_float;
		max_speed_int = distance_int;
		sprintf(max_speed, "%d", (int)distance_float);

		// Else, add distance between last two points to total distance and
		// Check if maximum speed is less than speed between last two points
	} else {
		distance_int += (int)distance_float;
		if (max_speed_int < (int)distance_float) {
			max_speed_int = (int)distance_float;
			sprintf(max_speed, "%d", (int)distance_float);
		}
	}

	sprintf(distance, "%d", distance_int);
}

/*
 * Store the average speed of the session into average_speed
 */
void getAverageSpeed(char *average_speed) {
	sprintf(average_speed, "%d", (distance_int / time_elapsed_seconds));
}

/*
 * Store the current field from the GPS, and its unit (the next field) in field_with_unit
 */
void getFieldWithUnit(char *field_with_unit) {
	char field[DATASIZE];
	getField(field);

	char unit[DATASIZE];
	getField(unit);

	sprintf(field_with_unit, "%s %s", field, unit);
}

/*
 * Get the session's start time, time elapsed, starting points, ending points, total distance, and average speed
 * Push this data into the database
 */
void getSessionData(void) {
	printf("Start time: %s ", start_time);
	printf("Time Elapsed: %s ", time_elapsed);
	printf("Start Latitude: %s ", start_latitude);
	printf("Start Longitude: %s ", start_longitude);
	printf("End Latitude: %s ", latitude);
	printf("End Longitude: %s ", longitude);
	printf("Total Distance: %s ", distance);

	getAverageSpeed(average_speed);
	printf("Average Speed: %s, \n\n", average_speed);

	// Get current session count
	char command[500];
	char response[500];

	createGetPrevSessionCountCommand(command);
	printf("command built: %s\n", command);
	sendCommand(command);
	usleep(5000);
	waitForAPIResponse(64, response);
	printf("Prev Ses Response:\n %s \n", response);
	parseCount(response);

	// Parse count from response;
	int prevSessionCount;
	int i;
	prevSessionCount = atoi(cur_session);
	printf("\n\nCurrent index found : %d", prevSessionCount);
	prevSessionCount++;

	// invoke wifi function here
	char insertCommand[500];
	char insertResponse[500];
	char id[500];

	sprintf(id, "%d", prevSessionCount);
	strcat(id, "_session");

	createInsertCommand(id, start_latitude, start_longitude, latitude, longitude, start_time, time_elapsed, distance, average_speed, insertCommand);
	printf("command built: %s\n", insertCommand);
	sendCommand(insertCommand);
	usleep(5000);
	waitForAPIResponse(64, insertResponse);

	char updateCommand[500];
	char updateResponse[500];
	char newCount[500];
	sprintf(newCount, "%d", prevSessionCount);

	createUpdatePrevSessionCountCommand(newCount, updateCommand);
	printf("command built: %s\n", updateCommand);
	sendCommand(updateCommand);
	usleep(5000);
	waitForAPIResponse(64, updateResponse);



}

/*
 * Covert latitude and longitude to the screen's x, y coordinates
 */
void latitudeLongitudeToXY(float lat1, float long1, int *x, int *y) {

	// Calculate the lengths of each latitude and longitude per pixel
	double latitude_pixel = (TOP_LEFT_LAT - BOTTOM_RIGHT_LAT) / HEIGHT;
	double longitude_pixel = (BOTTOM_RIGHT_LONG - TOP_LEFT_LONG) / LENGTH;
	*x = (long1 - TOP_LEFT_LONG) / longitude_pixel + MAP_X_OFFSET;
	*y = (TOP_LEFT_LAT - lat1) / latitude_pixel + MAP_Y_OFFSET;
}

/*
 * Draw a path between the last two logged points
 */
void drawPath(void) {
	//int x1, y1, x2, y2;
	int x1;
	int y1;
	int x2;
	int y2;

	// Draw the path between the last two hard-coded points
	if (test_index > 0) {

		latitudeLongitudeToXY(previous_latitude_float, previous_longitude_float, &x1, &y1);
		latitudeLongitudeToXY(latitude_float, longitude_float, &x2, &y2);

		WriteLine(x1, x2, y1, y2, 255);
		Circle(x2, y2, 10, 255);
	}

	// Increment the index of the hard-coded points, for testing and demo purposes
	test_index++;

	// There are 20 hard-coded points, reset index
	if (test_index == 20) {
		test_index = 1;
	}
}

/*
 * Store and calculate data from one $GPGGA log, and draw the path between its logged point and the previous point
 */
void PrintLog(void) {
	char startArray[7] = "$GPGGA,";
	int startArray_idx = 0;

	char c;
	int extracted_log = 0;

	// Extract characters until the log has been extracted
	while (extracted_log == 0) {
		c = getDataGPS();

		// Check for $GPGGA,
		if (c != startArray[startArray_idx]) {
			startArray_idx = 0;
		} else {
			startArray_idx++;
			if (startArray_idx == 7) {
				getTime(current_time);
				printf("Time: %s, ", current_time);

				getTimeElapsed(time_elapsed);
				printf("Time Elapsed: %s, ", time_elapsed);

				getLatitude(latitude);
				printf("Latitude: %s, ", latitude);

				getLongitude(longitude);
				printf("Longitude: %s, ", longitude);

				getField(fix);

				getField(satellites);

				getField(HDOP);

				getFieldWithUnit(altitude);

				getFieldWithUnit(geoidalSeparation);

				getDistanceAndSpeed(distance, speed);
				printf("Distance: %s, ", distance);
				printf("Speed: %s \n\n", speed);

				// Session has started: draw path, set previous point as the current point
				if (session_started == 1) {
					drawPath();
					previous_latitude_float = latitude_float;
					previous_longitude_float = longitude_float;
					if (extracted_first_log == 0) {
						extracted_first_log = 1;
					}
				}

				extracted_log = 1;
			}
		}
	}
}
