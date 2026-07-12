#include "IpMenu.hpp"

#include "Config.hpp"
#include "LiquidCrystalI2C.hpp"

#include <cstdio>

namespace Menu {

enum {
    LINE_BUFF_SIZE = LiquidCrystalI2C::COLS + 1,
};

IpMenu::IpMenu(CommonStuff const& common)
    : Menu(common) {}

IpMenu::~IpMenu() {}

char const* IpMenu::name() { return "IP ADDRESS"; }

void IpMenu::up(Button::Event e) {
    if (e != Button::Event::PRESS && e != Button::Event::REPEAT)
        return;
    if (_editIdx < 4) {
        ++_ipParts[_editIdx];
        _displayIp();
    } else {
        if (_subnet < 24)
            _subnet += 8;
        _displaySubnet();
    }
    _moveCursor();
}

void IpMenu::down(Button::Event e) {
    if (e != Button::Event::PRESS && e != Button::Event::REPEAT)
        return;
    if (_editIdx < 4) {
        --_ipParts[_editIdx];
        _displayIp();
    } else {
        if (_subnet > 8)
            _subnet -= 8;
        _displaySubnet();
    }
    _moveCursor();
}

void IpMenu::right(Button::Event e) {
    if (e != Button::Event::PRESS)
        return;
    if (_editIdx == 4) {
        applyConfig();
        return;
    }
    ++_editIdx;
    _moveCursor();
}

void IpMenu::_applyConfig() {
    _common.config->setNetwork(
            (uint32_t(_ipParts[0]) << 24) | (uint32_t(_ipParts[1]) << 16) |
            (uint32_t(_ipParts[2]) << 8) | (uint32_t(_ipParts[3]) << 0),
            _subnet);
}

void IpMenu::_enable() {
    uint32_t ip = _common.config->ip();
    _ipParts[0] = (ip >> 24) & 0xff;
    _ipParts[1] = (ip >> 16) & 0xff;
    _ipParts[2] = (ip >> 8) & 0xff;
    _ipParts[3] = (ip >> 0) & 0xff;

    _subnet = _common.config->subnet();
    _subnet = 8;

    _editIdx = 0;
    _common.screen->setCursorDisplay(true);
    _common.screen->setCursorBlink(true);
    _display();
}

void IpMenu::_disable() {
    _common.screen->setCursorDisplay(false);
    _common.screen->setCursorBlink(false);
}

void IpMenu::_display() {
    _common.screen->printLine(0, "IP");
    _common.screen->printLine(2, "Netmask");
    _displayIp();
    _displaySubnet();
    _moveCursor();
}

void IpMenu::_displayIp() {
    char buf[LINE_BUFF_SIZE];
    snprintf(buf, sizeof(buf), "  %3u.%3u.%3u.%3u", _ipParts[0], _ipParts[1], _ipParts[2],
             _ipParts[3]);
    _common.screen->printLine(1, buf);
}

void IpMenu::_displaySubnet() {
    uint32_t const subnetMask = (_subnet ? ~(uint64_t(1 << (32 - _subnet)) - 1) : 0);
    uint8_t const subnetMaskExploded[4] = {
        uint8_t(subnetMask >> 24),
        uint8_t(subnetMask >> 16),
        uint8_t(subnetMask >> 8),
        uint8_t(subnetMask >> 0),
    };
    char buf[LINE_BUFF_SIZE];
    snprintf(buf, sizeof(buf), "  %3u.%3u.%3u.%3u", subnetMaskExploded[0], subnetMaskExploded[1],
             subnetMaskExploded[2], subnetMaskExploded[3]);
    _common.screen->printLine(3, buf);
}

void IpMenu::_moveCursor() {
    if (_editIdx < 4)
        _common.screen->setCursor(_editIdx * 4 + 4, 1);
    else
        _common.screen->setCursor(16, 3);
}

} // namespace Menu
