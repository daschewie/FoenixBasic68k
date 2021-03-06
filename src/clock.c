#include <stdio.h>
#include <string.h>
#include "clock.h"
#include "mcp/syscalls.h"

void clock_set_date(uint16_t month, uint16_t day, uint16_t year) {
    t_time time;    
    sys_rtc_get_time(&time);

    // set the date portion
    time.month = month;
    time.day = day;
    time.year = year;
    sys_rtc_set_time(&time);

}

void clock_set_time(uint16_t h24, uint16_t min, uint16_t sec) {
    t_time time;
    sys_rtc_get_time(&time);

    // set the time portion
    time.hour = h24;
    time.minute = min;
    time.second = sec;
    time.is_24hours = 1;
    sys_rtc_set_time(&time);

}

void clock_date_str(char *buffer) {
    t_time time;
    sys_rtc_get_time(&time);
    sprintf(buffer, "%04d-%02d-%02d", time.year, time.month, time.day);
}

void clock_time_str(char *buffer) {
    t_time time;
    sys_rtc_get_time(&time);
    sprintf(buffer, "%d:%d:%d", time.hour, time.minute, time.second);
}

long clock_ticks() {
    return sys_time_jiffies();
}