#include "StatsMenu.hpp"

#include "LiquidCrystalI2C.hpp"
#include "Stats.hpp"

#include "main.h"

extern Stats a2d6Stats;

namespace Menu {

StatsMenu::StatsMenu(CommonStuff const& common) : Menu(common) {}
StatsMenu::~StatsMenu() {}

char const* StatsMenu::name() { return "Stats show"; }

void StatsMenu::up(Button::Event e) {}

void StatsMenu::down(Button::Event e) {}

void StatsMenu::right(Button::Event e) {}

void StatsMenu::tick() {
    if (!_common.screen->ready())
        return;

    auto now = HAL_GetTick();
    auto elapsed = now - _prevRefreshTime;

    if (elapsed < 100)
        return;

    _prevRefreshTime += elapsed - (elapsed % 100);
    _display();
}

void StatsMenu::_enable() {
    _display();
    _prevRefreshTime = HAL_GetTick();
}

void StatsMenu::_disable() {}

void StatsMenu::_display() {
    _common.screen->printLine(0, "StaaaaaaaaAAATTS");
    _common.screen->printLine(1, " ticks : %u", a2d6Stats.tickCount());
    _common.screen->printLine(2, " elapsd : %u", a2d6Stats.elapsedTimeMs());
    _common.screen->printLine(3, " w, m : %u, %u", a2d6Stats.writeCount(), a2d6Stats.moveCursorCount());
}

} // namespace Menu
