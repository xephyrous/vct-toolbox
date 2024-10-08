#ifndef _TIMER_H
#define _TIMER_H

#include <chrono>
#include <string>

class timer {
public:
    timer() = default;
    
    void startTimer();
    std::string resetTimer();
    
private:
    std::chrono::time_point<std::chrono::steady_clock> startClock;
    std::chrono::time_point<std::chrono::steady_clock> endClock;
};

#endif