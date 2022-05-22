#ifndef CLOCK_H
#define CLOCK_H

void clock_set_date(int month, int day, int year);
void clock_set_time(int h24, int min, int sec);
void clock_date_str(char *buffer);
void clock_time_str(char *buffer);
long clock_ticks();

#endif