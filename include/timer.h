#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

// Return current time in milliseconds
long Timer_currentMillis(void);

// Check timeout in milliseconds
bool Timer_checkTimeout(long start_time_ms, int timeout_ms);

// Wait time_ms milliseconds
void Timer_msPause(int time_ms);

#endif
