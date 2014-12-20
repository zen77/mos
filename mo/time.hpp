/* 
 * File:   time.h
 * Author: morgan
 *
 * Created on July 25, 2014, 11:51 AM
 */

#ifndef MO_TIME_H
#define	MO_TIME_H

#include <chrono>

namespace mo {

    // New implementation, uses c++11 std. Portability?
    /**
     * 
     * @return Current time in milliseconds. 
     */
    static double now_ms() {
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		return (double)millis;
	}

    /* Old implementation (Available on POSIX systems)
    static double now_ms(void) {
        struct timespec res;
        clock_gettime(CLOCK_REALTIME, &res);
        return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
    }
    */
}

#endif	/* TIME_H */
