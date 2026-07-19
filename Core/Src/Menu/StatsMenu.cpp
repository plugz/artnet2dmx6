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
//    if (!_common.screen->ready())
//        return;

    if (!_refreshTimer.done())
        return;
    _refreshTimer.advance();

    _display();
}

void StatsMenu::_enable() {
    _display();
    _refreshTimer.reset();
}

void StatsMenu::_disable() {}

void StatsMenu::_display() {
    _common.screen->printLine(0, "t:%u ms:%u", a2d6Stats.tickCount(), Chrono::MsClock::now().time_since_epoch().count());
    _common.screen->printLine(1, "c0:%u, c1:%u", a2d6Stats.counter(0), a2d6Stats.counter(1));
    _common.screen->printLine(2, "c2:%u, c3:%u", a2d6Stats.counter(2), a2d6Stats.counter(3));
    _common.screen->printLine(3, "c4:%u, c5:%u", a2d6Stats.counter(4), a2d6Stats.counter(5));
}

} // namespace Menu
