#ifndef __CHRONO_HPP__
#define __CHRONO_HPP__

#include <chrono>
#include <cstdint>

#include "main.h"

using Milliseconds = std::chrono::duration<uint32_t, std::ratio<1, 1000>>;
using Microseconds = std::chrono::duration<uint32_t, std::ratio<1, 1000000>>;

template<typename TDuration>
struct Clock
{
    using duration = TDuration;
    using rep = TDuration::rep;
    using period = TDuration::period;
    using time_point = std::chrono::time_point<Clock>;

    inline static time_point now();

    static constexpr bool is_steady =             false;
};

using MsClock = Clock<Milliseconds>;
using UsClock = Clock<Microseconds>;

using MsTimePoint = MsClock::time_point;
using UsTimePoint = UsClock::time_point;

template<>
inline MsTimePoint MsClock::now() {
    return time_point(duration(HAL_GetTick()));
}

template<>
inline UsTimePoint UsClock::now() {
    return time_point(duration(HAL_GetTick())); // TODO TODO TODOOOOOooo
}

template<typename TClock>
class Timer {
public:
    Timer(TClock::duration const& period) : _period(period) {}

    void reset() {
        _startTime = TClock::now();
    }

    void reset(TClock::duration const& newPeriod) {
        reset();
        _period = newPeriod;
    };

    void advance() {
        _startTime += _period;
    }

    TClock::duration elapsedTime() {
        return TClock::now() - _startTime;
    }

    bool done() {
        // handle overflow automagically thanks to unsigned arithmetic
        return TClock::now() - _startTime >= _period;
    }

    TClock::duration _period;
    TClock::time_point _startTime;

};

using MsTimer = Timer<MsClock>;
using UsTimer = Timer<UsClock>;

#endif
