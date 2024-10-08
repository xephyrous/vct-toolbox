#include "timer.h"

#include <sstream>

void timer::startTimer() {
    startClock = std::chrono::high_resolution_clock::now();
}

std::string timer::resetTimer() {
    std::ostringstream timeStr;

    endClock = std::chrono::high_resolution_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(endClock - startClock);
    
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(milliseconds);
    milliseconds -= std::chrono::duration_cast<std::chrono::milliseconds>(seconds);

    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);
    seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);

    if (minutes.count() >= 60) {
        const auto hours = std::chrono::duration_cast<std::chrono::hours>(minutes);
        minutes -= std::chrono::duration_cast<std::chrono::minutes>(hours);

        timeStr << hours.count() << "h ";
    }

    timeStr << minutes.count() << "m " << seconds.count() << "s " << milliseconds.count() << "ms\n";
    return timeStr.str();
}