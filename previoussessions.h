#include <stdio.h>
#include "Gps.h"

#define MAX_SESSIONS 10
int num_sessions;
char cur_session[DATASIZE];
char stime_prev[MAX_SESSIONS][50]; // num_sessions or DATASIZE
char time_elapsed_prev[MAX_SESSIONS][DATASIZE];
char start_latitude_prev[MAX_SESSIONS][DATASIZE];
char start_longitude_prev[MAX_SESSIONS][DATASIZE];
char end_latitude_prev[MAX_SESSIONS][DATASIZE];
char end_longitude_prev[MAX_SESSIONS][DATASIZE];
char distance_prev[MAX_SESSIONS][DATASIZE];
char average_speed_prev[MAX_SESSIONS][DATASIZE];

void getPreviousSessions(void);

