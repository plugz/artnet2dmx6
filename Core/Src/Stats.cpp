#include "Stats.hpp"

#include "main.h"

void Stats::tick() {
    _elapsedTimeMs = HAL_GetTick();
    ++_tickCount;
}
