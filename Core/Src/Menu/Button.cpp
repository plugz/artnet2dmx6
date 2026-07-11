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
        _pressTime = HAL_GetTick();
        _prevSendRepeatTime = _pressTime;
    } else
        (Menu::current()->*_cb)(Event::RELEASE);
}

void Button::tick() {
    if (!_pressed)
        return;

    auto elapsedTime = pressTime();

    if (elapsedTime > LONG_LONG_PRESS_TIME)
        _sendRepeat(LONG_LONG_PRESS_PERIOD);
    else if (elapsedTime > LONG_PRESS_TIME)
        _sendRepeat(LONG_PRESS_PERIOD);
}

void Button::_sendRepeat(uint32_t period) {
    auto now = HAL_GetTick();
    if (now >= _prevSendRepeatTime + period) {
        (Menu::current()->*_cb)(Event::REPEAT);
        _prevSendRepeatTime += period * ((now - _prevSendRepeatTime) / period);
    }
}

bool Button::pressed() const { return _pressed; }

uint32_t Button::pressTime() const { return HAL_GetTick() - _pressTime; }

} // namespace Menu
