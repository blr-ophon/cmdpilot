#include "timer.h"


// Return current time in milliseconds
long Timer_currentMillis(void) {
	struct timeval tp;

	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000 + tp.tv_usec / 1000;
	// Convert the seconds to milliseconds by multiplying by 1000
	// Convert the microseconds to milliseconds by dividing by 1000
}


// Check timeout in milliseconds
bool Timer_checkTimeout(long start_time, int timeout_ms){
    long current_time = Timer_currentMillis();
    long elapsed_ms = current_time - start_time;
                                                            
    //printf("TIME. start(%d), timeout_ms(%d), current(%d)\n",
    //        start_time, timeout_ms, current_time);

    if (elapsed_ms >= timeout_ms) {
        return 1;  // Timeout occurred
    } else {
        return 0;  // Timeout not yet occurred
    }
}


// Wait time_ms milliseconds
void Timer_msPause(int time_ms){
    //intra-packet pause (1ms)
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1000000L *time_ms; // 1 millisecond = 1,000,000 nanoseconds
    nanosleep(&ts, NULL);
}

