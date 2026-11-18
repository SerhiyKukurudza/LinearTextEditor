#include "dateTime.h"
#include <time.h>
#include <stdio.h>

void dateTimePrint(DateTime dt) {
    if (dateTimeIsEmpty(dt)) {
        printf("No modifications\n");
    } else {
        printf("%.2d/%.2d/%.4d %.2d:%.2d:%.2d\n", 
            dt.day, dt.month, dt.year, dt.hour, dt.minute, dt.second);
    }
}

DateTime dateTimeCreate() {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime); 

    DateTime dt;
    dt.day = timeinfo->tm_mday;
    dt.month = timeinfo->tm_mon + 1;     // tm_mon is 0-11
    dt.year = timeinfo->tm_year + 1900;  // tm_year is years since 1900
    dt.hour = timeinfo->tm_hour;
    dt.minute = timeinfo->tm_min;
    dt.second = timeinfo->tm_sec;

    return dt;
}

bool dateTimeIsEmpty(DateTime dt) {
    return dt.year == 0 && dt.month == 0 && dt.day == 0 &&
           dt.hour == 0 && dt.minute == 0 && dt.second == 0;
}

