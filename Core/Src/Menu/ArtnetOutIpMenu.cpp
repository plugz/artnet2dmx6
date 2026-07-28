#include "ArtnetOutIpMenu.hpp"

#include "Config.hpp"
#include "LiquidCrystalI2C.hpp"

#include <cstdio>

namespace Menu {

enum {
    LINE_BUFF_SIZE = LiquidCrystalI2C::COLS + 1,
};

ArtnetOutIpMenu::ArtnetOutIpMenu(CommonStuff const& common)
    : Menu(common) {}

ArtnetOutIpMenu::~ArtnetOutIpMenu() {}

char const* ArtnetOutIpMenu::name() { return "Set Target IP"; }

void ArtnetOutIpMenu::up(Button::Event e) {
    if (e != Button::Event::PRESS && e != Button::Event::REPEAT)
        return;
    ++_ipParts[_editIdx];
    _displayIp();
    _moveCursor();
}

void ArtnetOutIpMenu::down(Button::Event e) {
    if (e != Button::Event::PRESS && e != Button::Event::REPEAT)
        return;
    --_ipParts[_editIdx];
    _displayIp();
    _moveCursor();
}

void ArtnetOutIpMenu::left(Button::Event e) {
    if (e != Button::Event::PRESS)
        return;
    if (_editIdx == 0) {
        Menu::left(e);
        return;
    }
    --_editIdx;
    _moveCursor();
}

void ArtnetOutIpMenu::right(Button::Event e) {
    if (e != Button::Event::PRESS)
        return;
    if (_editIdx == 3) {
        applyConfig();
        return;
    }
    ++_editIdx;
    _moveCursor();
}

void ArtnetOutIpMenu::_applyConfig() {
    _common.config->setArtnetOutTargetIp(
            (uint32_t(_ipParts[0]) << 24) | (uint32_t(_ipParts[1]) << 16) |
            (uint32_t(_ipParts[2]) << 8) | (uint32_t(_ipParts[3]) << 0));
}

void ArtnetOutIpMenu::_enable() {
    uint32_t ip = _common.config->artnetOutTargetIp();
    _ipParts[0] = (ip >> 24) & 0xff;
    _ipParts[1] = (ip >> 16) & 0xff;
    _ipParts[2] = (ip >> 8) & 0xff;
    _ipParts[3] = (ip >> 0) & 0xff;

    _editIdx = 0;
    _common.screen->setCursorDisplay(true);
    _common.screen->setCursorBlink(true);
    _display();
}

void ArtnetOutIpMenu::_disable() {
    _common.screen->setCursorDisplay(false);
    _common.screen->setCursorBlink(false);
}

void ArtnetOutIpMenu::_display() {
    _common.screen->printLine(0, "Target IP");
    _common.screen->printLine(1, "");
    _common.screen->printLine(3, "for Artnet Out");
    _displayIp();
    _moveCursor();
}

void ArtnetOutIpMenu::_displayIp() {
    char buf[LINE_BUFF_SIZE];
    snprintf(buf, sizeof(buf), "  %3u.%3u.%3u.%3u", _ipParts[0], _ipParts[1], _ipParts[2], _ipParts[3]);
    _common.screen->printLine(2, buf);
}

void ArtnetOutIpMenu::_moveCursor() {
    _common.screen->setCursor(_editIdx * 4 + 4, 2, false);
}

} // namespace Menu

