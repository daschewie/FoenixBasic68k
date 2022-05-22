#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

void clock_set_date(uint16_t month, uint16_t day, uint16_t year);
void clock_set_time(uint16_t h24, uint16_t min, uint16_t sec);
void clock_date_str(char *buffer);
void clock_time_str(char *buffer);
long clock_ticks();

#endif