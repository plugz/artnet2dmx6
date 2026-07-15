#ifndef __CHRONO_HPP__
#define __CHRONO_HPP__

#include <chrono>
#include <cstdint>

//#include "main.h"
#include "tim.h"

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
    inline static void delay(duration const& d) {
        auto startPoint = now();
        while (now() - startPoint < d) {}
    }

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
    return time_point(duration(__HAL_TIM_GET_COUNTER(&htim5)));
}

struct Chrono {
    template<typename TDuration>
    static inline void delay(TDuration const& duration) {
        Clock<TDuration>::delay(duration);
    }

    static inline void init() {
        HAL_TIM_Base_Start(&htim5);
    }
};

template<typename TClock>
class Timer {
public:
    Timer(TClock::duration const& period) : _period(period) {}
    Timer() : _period(0) {}

    void reset() {
        _startTime = TClock::now();
    }

    void reset(TClock::duration const& newPeriod) {
        reset();
        _period = newPeriod;
    };

    void advance(TClock::duration const& period) {
        _startTime += period;
//        auto now = TClock::now();
//        if (_startTime + period < now)
//            _startTime = now;
    }

    void advance() {
        advance(_period);
    }

    TClock::duration elapsedTime() {
        return TClock::now() - _startTime;
    }

    bool done(TClock::duration const& period) {
        // handle overflow automagically thanks to unsigned arithmetic
        return TClock::now() - _startTime >= period;
    }

    bool done() {
        return done(_period);
    }

    TClock::duration _period;
    TClock::time_point _startTime;

};

using MsTimer = Timer<MsClock>;
using UsTimer = Timer<UsClock>;

#endif
