#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DrawChar.h"
#include "Gps.h"
#include "Colours.h"
#include "graphics.h"
#include "wifi.h"
#include "json.h"
#include <time.h>
#include <string.h>
#include "json.h"
#include "map.h"
#include "previoussessions.h"

char *lockedAchievementString = "LOCKED";

/**********************************************************************
* This function writes a single pixel to the x,y coords specified in the specified colour
* Note colour is a palette number (0-255) not a 24 bit RGB value
**********************************************************************/
void WriteAPixel (int x, int y, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = PutAPixel;		// give graphics a "write pixel" command
}

void WriteHLine (int x1, int x2, int y, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x1;			// write coords to x1, y1
	GraphicsX2Reg = x2;
	GraphicsY1Reg = y;

	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawHLine;		// give graphics a "draw Horizontal Line" command
}

void WriteVLine (int x, int y1, int y2, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y1;
	GraphicsY2Reg = y2;

	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawVLine;		// give graphics a "draw Horizontal Line" command
}

void WriteLine (int x1, int x2, int y1, int y2, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x1;			// write coords to x1, y1
	GraphicsX2Reg = x2;
	GraphicsY1Reg = y1;
	GraphicsY2Reg = y2;

	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawLine;		// give graphics a "draw Horizontal Line" command
}

/*****************************************************************************************
* This function read a single pixel from x,y coords specified and returns its colour
* Note returned colour is a palette number (0-255) not a 24 bit RGB value
******************************************************************************************/
int ReadAPixel (int x, int y)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsCommandReg = GetAPixel;		// give graphics a "get pixel" command

	WAIT_FOR_GRAPHICS;			// is graphics done reading pixel
	return (int)(GraphicsColourReg) ;		// return the palette number (colour)
}

/****************************************************************************************************
** subroutine to program a hardware (graphics chip) palette number with an RGB value
** e.g. ProgramPalette(RED, 0x00FF0000) ;
****************************************************************************************************/

void ProgramPalette(int PaletteNumber, int RGB)
{
	WAIT_FOR_GRAPHICS;
	GraphicsColourReg = PaletteNumber;
	GraphicsX1Reg = RGB >> 16   ;          // program red value in ls.8 bit of X1 reg
	GraphicsY1Reg = RGB ;                	 // program green and blue into 16 bit of Y1 reg
	GraphicsCommandReg = ProgramPaletteColour;	// issue command
}

/*
 * Function to clear the screen by drawing a black rectangle for the whole screen
 */
void ClearScreen()
{
	int i;
	for (i = 0; i < 480; i++)
		WriteHLine(0, 799, i, BLACK);
}

/*
 * Function that draws rectangle by drawing a line from x1 to x2 froom y1 to y2
 */
void Rectangle(int x1, int x2, int y1, int y2, int Colour)
{
	int i;
	for (i = y1; i < y2 + 1; i++)
		WriteHLine(x1, x2, i, Colour);
}

/*
 * Function that draws circle at given coordinate x and y with given radious
 */
void Circle(int x, int y, int radius, int Colour)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsX2Reg = radius;
	GraphicsY1Reg = y;

	GraphicsColourReg = Colour;		// set pixel colour with a palette number
	GraphicsCommandReg = DrawCircle;		// give graphics a "draw Horizontal Line" command
}

void initializeColours() {
	int i, j, k;
	for (i = 0 ; i < 4; i++) {
		ProgramPalette(i, 0x55 * i);
	}
	for (i = 1; i < 8; i++) {
		for (j = 0; j < 4; j++)
			ProgramPalette(4 * i + j, (0x24 * i << 8) + 0x55 * j);
	}
	for (i = 1; i < 8; i++) {
		for (j = 1; j < 8; j++) {
			for (k = 0; k < 4; k++)
				ProgramPalette(32 * i + 4 * j + k, (0x24 * i << 16) + (0x24 * j << 8) + 0x55 * k);
		}
	}
}

/*
 * Function that displays text on the screen by given x and y values and text color
 */
void drawString(char *string, int x, int y, int fontcolour, int backgroundcolour) {
	int i;
	for (i = 0; i < strlen(string); i++) {
		OutGraphicsCharFont2a( x + i * 15, y, fontcolour, backgroundcolour, string[i], 0);
	}
}

void drawStringSmallFont(char *string, int x, int y, int fontcolour, int backgroundcolour) {
	int i;
	for (i = 0; i < strlen(string); i++) {
		OutGraphicsCharFont1(x + i * 10, y, fontcolour, backgroundcolour, string[i], 0);
	}
}

/*
 * Draw Home Functions
 */
void drawHome() {
	ClearScreen();
	Rectangle(2, 397, 2, 237, MIDNIGHT_BLUE);
	Rectangle(402, 797, 2, 237, MAROON);
	Rectangle(2, 397, 242, 477, DARK_SEA_GREEN);
	Rectangle(402, 797, 242, 477, PINK);

	char* curSession = "CURRENT SESSION";
	char* prevSession = "PREVIOUS SESSIONS";
	char* achievementString = "ACHIEVEMENTS";
	char* callString = "CALL";

	drawString(curSession, 80, 118, WHITE, BLACK);
	drawString(prevSession, 470, 118, WHITE, BLACK);
	drawString(achievementString, 510, 358, WHITE, BLACK);
	drawString(callString, 170, 358, WHITE, BLACK);
}

/*
 * Draw Current Session Functions
 */
void drawLog() {

	// Draw the time that is displayed on Current Session
	char time_string[DATASIZE];
	sprintf(time_string, "Time: %s, Time Elapsed: %s", current_time, time_elapsed);
	drawString(time_string, 10, 10, BLACK, WHITE);
	// Draw the Longitude, Latitude and Altitude that is displayed on Current Session
	char position_string[4 * DATASIZE];
	sprintf(position_string, "Lat: %s, Long: %s, Alt: %s", latitude, longitude, altitude);
	drawString(position_string, 10, 30, BLACK, WHITE);
	// Draw the Distance and Speed that is displayed on Current Session
	char distance_speed_string[DATASIZE];
	sprintf(distance_speed_string, "Distance: %s M, Speed: %s M/S", distance, speed);
	drawString(distance_speed_string, 10, 50, BLACK, WHITE);
}

/*
 * Function that ereases the log printed out by GPS
 */
void eraseLogGUI() {
	Rectangle(2, 797, 2, 80, WHITE);
}

/*
 * Function that draws start button for current session
 */
void drawStartButton() {

	Rectangle(402, 797, 429, 477, FOREST_GREEN);

	char* startString = "START";
	drawString(startString, 570, 450, WHITE, BLACK);
}

/*
 * Function that draws stop button for current session
 */
void drawStopButton() {

	Rectangle(402, 797, 429, 477, MAROON);

	char *stopString = "STOP";
	drawString(stopString, 570, 450, WHITE, BLACK);
}

/*
 * Draw map of UBC in Current Session module
 */
void drawMap() {
	// For loops to program 256 Colour Palettes
	int i, j;

	// Draw the map using 8 hex arrays
	for (j = 0; j < IMAGE_HEIGHT; j++) {
		for (i = 0; i < IMAGE_LENGTH; i++) {
			WriteAPixel(i + MAP_X_OFFSET, j + MAP_Y_OFFSET, map00[i + IMAGE_LENGTH * j]);
			WriteAPixel(i + MAP_X_OFFSET + IMAGE_LENGTH, j + MAP_Y_OFFSET, map01[i + IMAGE_LENGTH * j]);
			WriteAPixel(i + MAP_X_OFFSET + 2 * IMAGE_LENGTH, j + MAP_Y_OFFSET, map02[i + IMAGE_LENGTH * j]);
			WriteAPixel(i + MAP_X_OFFSET + 3 * IMAGE_LENGTH, j + MAP_Y_OFFSET, map03[i + IMAGE_LENGTH * j]);
			WriteAPixel(i + MAP_X_OFFSET, j + IMAGE_HEIGHT + MAP_Y_OFFSET, map10[i + IMAGE_LENGTH * j]);
			WriteAPixel(i + MAP_X_OFFSET + IMAGE_LENGTH, j + IMAGE_HEIGHT + MAP_Y_OFFSET, map11[i + IMAGE_LENGTH * j]);
			WriteAPixel(i + MAP_X_OFFSET + 2 * IMAGE_LENGTH, j + IMAGE_HEIGHT + MAP_Y_OFFSET, map12[i + IMAGE_LENGTH * j]);
			WriteAPixel(i + MAP_X_OFFSET + 3 * IMAGE_LENGTH, j + IMAGE_HEIGHT + MAP_Y_OFFSET, map13[i + IMAGE_LENGTH * j]);
		}
	}
}

/*
 * Draw Current Session screen (before starting)
 */
void drawStartSession() {
	Rectangle(0, 799, 0, 479, BLACK);
	Rectangle(2, 797, 2, 80, WHITE);

	drawMap();

	// Home button
	Rectangle(2, 400, 429, 477, MIDNIGHT_BLUE);

	// Write the string for home
	char* homeString = "HOME";
	drawString(homeString, 170, 450, WHITE, BLACK);

	drawStartButton();
}

void drawEntry(char* stime, char* time_elapsed, char* start_latitude, char *start_longitude, char* end_latitude, char* end_longitude,
               char* distance, char* average_speed, int x, int y) {
	// Draw start time and time elapsed
	char time_string[DATASIZE];
	sprintf(time_string, "Start Time: %s, Time Elapsed: %s", stime, time_elapsed);
	drawString(time_string, x, y, BLACK, WHITE);

	// Draw the Distance and Speed
	char distance_speed_string[DATASIZE];
	sprintf(distance_speed_string, "Distance: %s M, Average Speed: %s M/S", distance, average_speed);
	drawString(distance_speed_string, x, y + 20, BLACK, WHITE);

	// Draw start position and end position
	char position_string[4 * DATASIZE];
	sprintf(position_string, "Started At: (%s, %s), Ended At: (%s, %s)", start_latitude, start_longitude, end_latitude, end_longitude);
	drawStringSmallFont(position_string, x, y + 40, BLACK, WHITE);
}

void drawListofSessions() {
	getPreviousSessions();

	int i;
	/* Test */
//	for (i = 0; i < 4; i++) {
//		strcpy(stime_prev[i], "12:00:00 PST");
//		strcpy(time_elapsed_prev[i], "0:0:1");
//		strcpy(start_latitude_prev[i], "49.021456");
//		strcpy(start_longitude_prev[i], "-123.123456");
//		strcpy(end_latitude_prev[i], "43.098765");
//		strcpy(end_longitude_prev[i], "-123.099876");
//		strcpy(distance_prev[i], "10 M");
//		strcpy(average_speed_prev[i], "10 M/S");
//	}

	int x = 20;
	int y = 20;

	for (i = 0; i < 4; i++) {
		if (i < num_sessions) {
			drawEntry(stime_prev[i], time_elapsed_prev[i], start_latitude_prev[i], start_longitude_prev[i], end_latitude_prev[i], end_longitude_prev[i], distance_prev[i], average_speed_prev[i], x, y + (i * 100));
		}
	}
}

/*
 * Draw Previous Session Screen
 */
void drawPreviouSession() {
	ClearScreen();
	// White background
		Rectangle(2, 797, 2, 427, BLACK);
		drawString("Loading...", 15, 15, WHITE, BLACK);

	// invoke wifi function here
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
	printf("\n\nFOUND SESSION NUMBER = %d\n\n", prevSessionCount);


	char curCommand[500];
	char curResponse[500];
	for (i = 0; i < prevSessionCount; i++)
	{
		char *id;
		sprintf(id, "%d", i);
		strcat(id, "_session");
		strcpy(curCommand, "");
		strcpy(curResponse, "");

		createGetCommand(id, curCommand);
		printf("command built: %s\n", curCommand);
		sendCommand(curCommand);
		usleep(5000);
		waitForAPIResponse(64, curResponse);
		printf("Prev Session #%d : %s\n", i, curResponse);
		parseSession(curResponse, i);
		// parse and save this session somewhere
	}

	// now draw the sessions screen with all the sessions obtained.

	ClearScreen();
	// Entries
	Rectangle(2, 797, 2, 102, WHITE);
	Rectangle(2, 797, 106, 206, WHITE);
	Rectangle(2, 797, 210, 310, WHITE);
	Rectangle(2, 797, 314, 414, WHITE);
	// Next Page Button
	Rectangle(403, 797, 429, 477, FOREST_GREEN);
	// Home Button
	Rectangle(2, 399, 429, 477, MIDNIGHT_BLUE);

	char* homeString = "HOME";
	char* nextString = "NEXT";
	drawString(homeString, 170, 450, WHITE, BLACK);
	drawString(nextString, 570, 450, WHITE, BLACK);

	drawListofSessions();

}

/*
 * Draw Call Screen Functions
 */
void drawKeypad() {
	ClearScreen();
	// Draw number area
	Rectangle(2, 797, 2, 80, WHITE);
	// Keypad
	// First Row
	Rectangle(2, 261, 85, 173, DARK_SEA_GREEN);
	Rectangle(265, 526, 85, 173, DARK_SEA_GREEN);
	Rectangle(530, 791, 85, 173, DARK_SEA_GREEN);
	// Second Row
	Rectangle(2, 261, 177, 259, DARK_SEA_GREEN);
	Rectangle(265, 526, 177, 259, DARK_SEA_GREEN);
	Rectangle(530, 791, 177, 259, DARK_SEA_GREEN);
	// Third Row
	Rectangle(2, 261, 263,  345, DARK_SEA_GREEN);
	Rectangle(265, 526, 263,  345, DARK_SEA_GREEN);
	Rectangle(530, 791, 263,  345, DARK_SEA_GREEN);
	// Fourth Row
	Rectangle(2, 261, 349, 425, DARK_SEA_GREEN);
	Rectangle(265, 526, 349, 425, DARK_SEA_GREEN);
	Rectangle(530, 791, 349, 425, DARK_SEA_GREEN);
	// Call Button
	Rectangle(403, 797, 429, 477, FOREST_GREEN);
	// Home Button
	Rectangle(2, 399, 429, 477, MIDNIGHT_BLUE);

	char* zero = "0";
	char* one = "1";
	char* two = "2";
	char* three = "3";
	char* four = "4";
	char* five = "5";
	char* six = "6";
	char* seven = "7";
	char* eight = "8";
	char* nine = "9";
	char* clear = "C";
	char* homeString = "HOME";
	char* callString = "CALL";

	// Writing the numbers onto the correct location of button
	OutGraphicsCharFont2a( 400, 380, WHITE, BLACK, zero[0], 0);
	OutGraphicsCharFont2a( 646, 380, WHITE, BLACK, clear[0], 0);

	OutGraphicsCharFont2a( 133, 294, WHITE, BLACK, one[0], 0);
	OutGraphicsCharFont2a( 400, 294, WHITE, BLACK, two[0], 0);
	OutGraphicsCharFont2a( 646, 294, WHITE, BLACK, three[0], 0);

	OutGraphicsCharFont2a( 133, 208, WHITE, BLACK, four[0], 0);
	OutGraphicsCharFont2a( 400, 208, WHITE, BLACK, five[0], 0);
	OutGraphicsCharFont2a( 646, 208, WHITE, BLACK, six[0], 0);

	OutGraphicsCharFont2a( 133, 116, WHITE, BLACK, seven[0], 0);
	OutGraphicsCharFont2a( 400, 116, WHITE, BLACK, eight[0], 0);
	OutGraphicsCharFont2a( 646, 116, WHITE, BLACK, nine[0], 0);

	drawString(homeString, 170, 450, WHITE, BLACK);
	drawString(callString, 570, 450, WHITE, BLACK);
}

/*
 * Clear the keypad number screen that displays the current phone number
 */
void clearNumberScreen() {
	Rectangle(2, 797, 2, 80, WHITE);
}

/*
 * Prints the number pressed on keypad
 */
void printDialNumber(char number, int dialIndex) {
	OutGraphicsCharFont2a(dialIndex, 40, BLACK, BLACK, number, 0);
}

/*
 * Write text that the achievements are initially locked
 */
void drawLockedAchievement(int x, int y) {
	int achievementsRadius = 80;
	Circle(x, y , achievementsRadius, BLACK);
	drawString(lockedAchievementString, x - 40, y - 5, WHITE, BLACK);
}

/*
 * Drawing the medal for unlocking first distance achievement
 */
void drawAchievementDistance1() {
	int achievementsRadius = 80;
	int x = 120;
	int y = 100;
	// Check the distance1_achieved flag to determine if achievement is locked or not
	if (distance1_achieved == 1) {
		Circle(x, y, achievementsRadius, PINK);
		char *achievementString1 = "100 M IN";
		drawString(achievementString1, x - 55, y - 15, WHITE, BLACK);
		char *achievementString2 = "A SESSION";
		drawString(achievementString2, x - 60, y + 5, WHITE, BLACK);

	} else {
		drawLockedAchievement(x, y);
	}
}

/*
 * Drawing the medal for unlocking second distance achievement
 */
void drawAchievementDistance2() {
	int achievementsRadius = 80;
	int x = 120;
	int y = 300;
	// Check the distance2_achieved flag to determine if achievement is locked or not
	if (distance2_achieved) {
		Circle(x, y, achievementsRadius, PINK);
		char *achievementString1 = "500 M IN";
		drawString(achievementString1, x - 55, y - 15, WHITE, BLACK);
		char *achievementString2 = "A SESSION";
		drawString(achievementString2, x - 60, y + 5, WHITE, BLACK);
	} else {
		drawLockedAchievement(x, y);
	}
}

/*
 * Drawing the medal for unlocking first session achievement
 */
void drawAchievementSession1() {
	int achievementsRadius = 80;
	int x = 399;
	int y = 100;
	// Check the session1_achieved flag to determine if achievement is locked or not
	if (session1_achieved) {
		Circle(x, y, achievementsRadius, TEAL);
		char *achievementString1 = "3 SESSIONS";
		drawString(achievementString1, x - 70, y - 15, WHITE, BLACK);
		char *achievementString2 = "COMPLETED";
		drawString(achievementString2, x - 60, y + 5, WHITE, BLACK);
	} else {
		drawLockedAchievement(x, y);
	}
}

/*
 * Drawing the medal for unlocking second session achievement
 */
void drawAchievementSession2() {
	int achievementsRadius = 80;
	int x = 399;
	int y = 300;
	// Check the session2_achieved flag to determine if achievement is locked or not
	if (session2_achieved) {
		Circle(x, y, achievementsRadius, TEAL);
		char *achievementString1 = "5 SESSIONS";
		drawString(achievementString1, x - 70, y - 15, WHITE, BLACK);
		char *achievementString2 = "COMPLETED";
		drawString(achievementString2, x - 60, y + 5, WHITE, BLACK);
	} else {
		drawLockedAchievement(x, y);
	}
}

/*
 * Drawing the medal for unlocking first speed achievement
 */
void drawAchievementSpeed1() {
	int achievementsRadius = 80;
	int x = 678;
	int y = 100;
	// Check the speed1_achieved flag to determine if achievement is locked or not
	if (speed1_achieved) {
		Circle(x, y, achievementsRadius, MEDIUM_VIOLET_RED);
		char *achievementString1 = "REACHED";
		drawString(achievementString1, x - 45, y - 15, WHITE, BLACK);
		char *achievementString2 = "3 M/S";
		drawString(achievementString2, x - 30, y + 5, WHITE, BLACK);
	} else {
		drawLockedAchievement(x, y);
	}
}

/*
 * Drawing the medal for unlocking second speed achievement
 */
void drawAchievementSpeed2() {
	int achievementsRadius = 80;
	int x = 678;
	int y = 300;
	// Check the speed2_achieved flag to determine if achievement is locked or not
	if (speed2_achieved) {
		Circle(x, y, achievementsRadius, MEDIUM_VIOLET_RED);
		char *achievementString1 = "REACHED";
		drawString(achievementString1, x - 45, y - 15, WHITE, BLACK);
		char *achievementString2 = "5 M/S";
		drawString(achievementString2, x - 30, y + 5, WHITE, BLACK);
	} else {
		drawLockedAchievement(x, y);
	}

}

/*
 * Draw Achievement Screen Functions
 */
void drawAchievementsScreen() {
	ClearScreen();
	// White background
	Rectangle(2, 797, 2, 427, BLACK);
	drawString("Loading...", 15, 15, WHITE, BLACK);

	// Download new Achievement states
	char command[500];
	char response[500];
	createGetCommand("achievement_states", command);
	printf("command built: %s\n", command);
	sendCommand(command);
	usleep(5000);
	waitForAPIResponse(64, response);
	//printf("response: %s\n Now checking if it's a success...\n", response);
	char *isSuccess;
	isSuccess = strstr(response, "200");
	if (isSuccess) {
		printf("Parsing achievement response into json decoder...\n");
		parseAchievements(response);
	}

	char countCommand[500];
	char countResponse[500];

	createGetPrevSessionCountCommand(countCommand);
	printf("command built: %s\n", countCommand);
	sendCommand(countCommand);
	usleep(5000);
	waitForAPIResponse(64, countResponse);
	printf("Prev Ses Response:\n %s \n", countResponse);
	parseCount(countResponse);

	// Parse count from response;
	int prevSessionCount;
	prevSessionCount = atoi(cur_session);
	printf("\n\nCurrent index found : %d", prevSessionCount);

	char curCommand[500];
	char curResponse[500];
	int i;
	for (i = 0; i < num_sessions; i++)
	{
		char *id;
		sprintf(id, "%d", i);
		strcat(id, "_session");
		strcpy(curCommand, "");
		strcpy(curResponse, "");

		createGetCommand(id, curCommand);
		printf("command built: %s\n", curCommand);
		sendCommand(curCommand);
		usleep(5000);
		waitForAPIResponse(64, curResponse);
		printf("Prev Session #%d : %s\n", i, curResponse);
		parseSession(curResponse, i);
		// parse and save this session somewhere
	}
/*
	char countCommand[500];
	char countResponse[500];
	createGetCommand("sessionCount", countCommand);
	printf("command built: %s\n", countCommand);
	sendCommand(countCommand);
	usleep(5000);
	waitForAPIResponse(64, countResponse);
	parseCount(countResponse);
*/
	// Re-upload new Achievement states
	if (atoi(distance_prev[num_sessions]) > 100){
		distance1_achieved = 1;
	}

	if (atoi(distance_prev[num_sessions]) > 500){
		distance2_achieved = 1;
	}

	if (num_sessions >= 2){
		session1_achieved = 1;
	}

	if (num_sessions >= 4){
		session2_achieved = 1;
	}

	if (atoi(average_speed_prev[num_sessions]) > 2){
		speed1_achieved = 1;
	}

	if (atoi(average_speed_prev[num_sessions]) > 4){
		speed2_achieved = 1;
	}
	// Parse response?


	// Home
	Rectangle(2, 797, 2, 427, WHITE);
	Rectangle(202, 599, 429, 477, MIDNIGHT_BLUE);

	// Draw the achievements depending if they are unlocked or locked
	drawAchievementDistance1();
	drawAchievementDistance2();
	drawAchievementSession1();
	drawAchievementSession2();
	drawAchievementSpeed1();
	drawAchievementSpeed2();

	char* homeString = "HOME";
	drawString(homeString, 370, 450, WHITE, BLACK);
	printf("Finished drawing achievements screen.\n");
}

/*
 * Function that checks if points on screen is within the screen
 */
int validPointOnScreen(int x, int y) {
	// Check if x lies to the left of screen bounds
	if (x < 0) {
		return 0;
	}
	// 799 is the screen width
	// Check if x lies to the right of screen bounds
	if (x > 799) {
		return 0;
	}

	// Check if y lies above screen bounds
	if (y < 0) {
		return 0;
	}
	// 479 is screen height
	// Check if y lies below screen bounds
	if (y > 479) {
		return 0;
	}

	return 1;
}
