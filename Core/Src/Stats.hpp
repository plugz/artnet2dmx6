#ifndef __STATS_HPP__
#define __STATS_HPP__

#include <cstdint>

class Stats {
public:
    enum {COUNTER_COUNT = 6};
public:
    void tick();

    inline uint32_t tickCount() const { return _tickCount; }

    void increaseCounter(int idx) { ++_counters[idx]; }
    void setCounter(int idx, uint32_t val) { _counters[idx] = val; }
    uint32_t counter(int idx) { return _counters[idx]; }

private:
    uint32_t _tickCount = 0;
    uint32_t _counters[COUNTER_COUNT];
};

#endif
