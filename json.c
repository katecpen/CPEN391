#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#include "json.h"
#include "previoussessions.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

void parseAchievements(char * JSON_STRING){
	 	int k;
	    char * result;
	    int count;
	    int size = strlen(JSON_STRING);
	    count = 0;
	    k = 10;
	    while(JSON_STRING[k] != '$'){
	        count++;
	        k++;
	    }

	    printf("%c\n", JSON_STRING[size-1]);
	    printf("count: %d, k: %d, input size: %d\n", count, k, strlen(JSON_STRING));
	    strncpy(result, JSON_STRING + 10, k);
	    result[count] = '\0';
	    printf("%s\n", result);
	    JSON_STRING = result;

	    int i;
	    int r;
	    jsmn_parser p;
	    jsmntok_t t[128]; /* We expect no more than 128 tokens */

	    jsmn_init(&p);
	    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	    if (r < 0) {
	        printf("Failed to parse JSON: %d\n", r);
	        return;
	    }

	    /* Assume the top-level element is an object */
	    if (r < 1 || t[0].type != JSMN_OBJECT) {
	        printf("Object expected\n");
	        return;
	    }

	    /* Loop over all keys of the root object */
	    for (i = 1; i < r; i++) {
	        if (jsoneq(JSON_STRING, &t[i], "_id") == 0) {
	            /* We may use strndup() to fetch string value */
	            printf("- ID: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "speed2") == 0) {
	            /* We may additionally check if the value is either "true" or "false" */
	            printf("- speed2: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *speed2;
	            sprintf(speed2, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",speed2);
	            speed2_achieved = atoi(speed2);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "session1") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- session1: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *session1;
	            sprintf(session1, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",session1);
	            session1_achieved = atoi(session1);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "distance1") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- distance1: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *distance1;
	            sprintf(distance1, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",distance1);
	            distance1_achieved = atoi(distance1);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "session2") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- session2: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *session2;
	            sprintf(session2, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",session2);
	            session2_achieved = atoi(session2);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "speed1") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- speed1: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *speed1;
	            sprintf(speed1, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",speed1);
	            speed1_achieved = atoi(speed1);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "numSessions") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- numSessions: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *numSessions;
	            sprintf(numSessions, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",numSessions);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "distance2") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- distance2: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *distance2;
	            sprintf(distance2, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",distance2);
	            distance2_achieved = atoi(distance2);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "achievementRadius") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- achievemenRadius: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *radius;
	            sprintf(radius, "*.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",radius);
	            achievementsRadius = 80;
	            i++;
	        }

	        else {
	            printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
	                   JSON_STRING + t[i].start);
	        }
	    }
	    return;
}


void parseSession(char * JSON_STRING, int index){
	 	int k;
	    char * result;
	    int count;
	    int size = strlen(JSON_STRING);
	    count = 0;
	    k = 10;
	    while(JSON_STRING[k] != '$'){
	        count++;
	        k++;
	    }

	    printf("JSON STRING: %s \n :end\n", JSON_STRING);
	    printf("JSON STRING - count: %d, k: %d, input size: %d\n", count, k, strlen(JSON_STRING));
	    strncpy(result, JSON_STRING + 10, k);
	    result[count] = '\0';
	    printf("JSON STRING PARSED : %s\n :end \n", result);
	    JSON_STRING = result;
	    printf("\n\n CURRENT SESSION INDEX : %d \n\n", index);

	    int i;
	    int r;
	    jsmn_parser p;
	    jsmntok_t t[128]; /* We expect no more than 128 tokens */

	    jsmn_init(&p);
	    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	    if (r < 0) {
	        printf("Failed to parse JSON: %d\n", r);
	        return;
	    }

	    /* Assume the top-level element is an object */
	    if (r < 1 || t[0].type != JSMN_OBJECT) {
	        printf("Object expected\n");
	        return;
	    }

	    //int sessionIndex = atoi(cur_session);

	    /* Loop over all keys of the root object */
	    for (i = 1; i < r; i++) {
	        if (jsoneq(JSON_STRING, &t[i], "_id") == 0) {
	            /* We may use strndup() to fetch string value */
	            printf("- ID: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "start_time") == 0) {
	            /* We may additionally check if the value is either "true" or "false" */
	            printf("- start_time: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *startTime;
	            //strncpy(startTime, JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start);
	            sprintf(startTime, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",startTime);
	            //sprintf(dist1, "%d", distance1_achieved);
	            strcpy(stime_prev[index], startTime);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "end_time") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- end_time: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *endTime;
	            sprintf(endTime, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",endTime);
	            strcpy(time_elapsed_prev[index], endTime);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "start_lat") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- start_lat: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *startLat;
	            sprintf(startLat, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",startLat);
	            strcpy(start_latitude_prev[index], startLat);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "start_long") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- start_long: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *start_long;
	            sprintf(start_long, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",start_long);
	            strcpy(start_longitude_prev[index], start_long);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "end_lat") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- end_lat: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *endLat;
	            sprintf(endLat, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",endLat);
	            strcpy(end_latitude_prev[index], endLat);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "end_long") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- end_long: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *endLong;
	            sprintf(endLong, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",endLong);
	            strcpy(end_longitude_prev[index], endLong);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "total_distance") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- total_distance: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *totalDistance;
	            sprintf(totalDistance, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",totalDistance);
	            strcpy(distance_prev[index], totalDistance);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "speed") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- speed: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *speed;
	            sprintf(speed, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",speed);
	            strcpy(average_speed_prev[index], speed);
	            i++;
	        }

	        else {
	            printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
	                   JSON_STRING + t[i].start);
	        }
	    }
	    return;
}

void parseCount(char * JSON_STRING){
	 	int k;
	    char * result;
	    int count;
	    int size = strlen(JSON_STRING);
	    count = 0;
	    k = 10;
	    while(JSON_STRING[k] != '$'){
	        count++;
	        k++;
	    }

	    printf("%c\n", JSON_STRING[size-1]);
	    printf("count: %d, k: %d, input size: %d\n", count, k, strlen(JSON_STRING));
	    strncpy(result, JSON_STRING + 10, k);
	    result[count] = '\0';
	    printf("%s\n", result);
	    JSON_STRING = result;

	    int i;
	    int r;
	    jsmn_parser p;
	    jsmntok_t t[128]; /* We expect no more than 128 tokens */

	    jsmn_init(&p);
	    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	    if (r < 0) {
	        printf("Failed to parse JSON: %d\n", r);
	        return;
	    }

	    /* Assume the top-level element is an object */
	    if (r < 1 || t[0].type != JSMN_OBJECT) {
	        printf("Object expected\n");
	        return;
	    }

	    /* Loop over all keys of the root object */
	    for (i = 1; i < r; i++) {
	        if (jsoneq(JSON_STRING, &t[i], "_id") == 0) {
	            /* We may use strndup() to fetch string value */
	            printf("- ID: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "count") == 0) {
	            /* We may additionally check if the value is either "true" or "false" */
	            printf("- start_time: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *prevCount;
	            //strncpy(startTime, JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start);
	            sprintf(prevCount, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",prevCount);
	            strcpy(cur_session, prevCount);
	            num_sessions = atoi(prevCount);
	            printf("Cursession: %s\n", cur_session);
	            //sprintf(dist1, "%d", distance1_achieved);
	            //speed2_achieved = atoi(startTime) / 10;
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "start_time") == 0) {
	            /* We may additionally check if the value is either "true" or "false" */
	            printf("- start_time: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *startTime;
	            //strncpy(startTime, JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start);
	            //sprintf(startTime, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",startTime);
	            //sprintf(dist1, "%d", distance1_achieved);
	            //speed2_achieved = atoi(startTime) / 10;
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "end_time") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- end_time: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *endTime;
	            //sprintf(endTime, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",endTime);
	            //session1_achieved = atoi(session1) / 10;
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "start_lat") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- start_lat: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *startLat;
	            //sprintf(startLat, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",startLat);
	            //distance1_achieved = atoi(distance1) / 10;
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "start_long") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- start_long: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *start_long;
	            //sprintf(start_long, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",start_long);
	            //session2_achieved = atoi(session2);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "end_lat") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- end_lat: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *endLat;
	            //sprintf(endLat, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",endLat);
	            //asda = atoi(endLat);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "end_long") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- end_long: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *endLong;
	            //sprintf(endLong, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",endLong);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "total_distance") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- total_distance: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *totalDistance;
	            //sprintf(totalDistance, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",totalDistance);
	            //distance2_achieved = atoi(totalDistance);
	            i++;
	        } else if (jsoneq(JSON_STRING, &t[i], "speed") == 0) {
	            /* We may want to do strtol() here to get numeric value */
	            printf("- speed: %.*s\n", t[i+1].end-t[i+1].start,
	                   JSON_STRING + t[i+1].start);
	            char *speed;
	            //sprintf(speed, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
	            printf("\n %s \n",speed);
	            i++;
	        }

	        else {
	            printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
	                   JSON_STRING + t[i].start);
	        }
	    }
	    return;
}

