#include "Button.hpp"

#include "Menu.hpp"

#include "main.h"

namespace Menu {

Button::Button(MenuCallback cb)
    : _pressed(false)
    , _cb(cb) {}

Button::~Button() {}

void Button::updateStatus(bool pressed) {
    _pressed = pressed;

    if (pressed) {
        (Menu::current()->*_cb)(Event::PRESS);
        _longPressTimer.reset();
        _longPressRepeatTimer.reset();
    } else
        (Menu::current()->*_cb)(Event::RELEASE);
}

void Button::tick() {
    if (!_pressed)
        return;

    if (_longPressTimer.done(LONG_LONG_PRESS_TIME)) {
        _sendRepeat(LONG_LONG_PRESS_REPEAT_PERIOD);
    }
    else if (_longPressTimer.done(LONG_PRESS_TIME)) {
        _sendRepeat(LONG_PRESS_REPEAT_PERIOD);
    }
}

void Button::_sendRepeat(Chrono::Milliseconds period) {
    if (!_longPressRepeatTimer.done(period))
        return;

    _longPressTimer.advance(period);
    if (_longPressTimer.done()) // avoid firing at each loop
        _longPressTimer.reset();

    (Menu::current()->*_cb)(Event::REPEAT);
}

bool Button::pressed() const { return _pressed; }

} // namespace Menu
