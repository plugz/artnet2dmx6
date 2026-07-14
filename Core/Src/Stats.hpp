#ifndef __STATS_HPP__
#define __STATS_HPP__

#include <cstdint>

class Stats {
public:
    void tick();

    void increaseWriteCount() { ++_writeCount; }
    void increaseMoveCursorCount() { ++_moveCursorCount; }

    inline uint32_t writeCount() { return _writeCount; }
    inline uint32_t moveCursorCount() { return _moveCursorCount; }
    inline uint32_t elapsedTimeMs() const { return _elapsedTimeMs; }
    inline uint32_t tickCount() const { return _tickCount; }

private:
    uint32_t _writeCount = 0;
    uint32_t _moveCursorCount = 0;
    uint32_t _elapsedTimeMs = 0;
    uint32_t _tickCount = 0;
};

#endif
