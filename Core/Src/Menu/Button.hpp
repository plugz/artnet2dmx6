#ifndef __MENU_BUTTON_HPP__
#define __MENU_BUTTON_HPP__

#include <cstdint>

namespace Menu {

class Menu;

class Button {
public:
    enum class Event {
        PRESS,
        REPEAT,
        RELEASE,
    };

public:
    using MenuCallback = void (Menu::*)(Event);

    static constexpr uint32_t LONG_PRESS_TIME = 500;
    static constexpr uint32_t LONG_LONG_PRESS_TIME = LONG_PRESS_TIME * 4;

    static constexpr uint32_t LONG_PRESS_PERIOD = 50;
    static constexpr uint32_t LONG_LONG_PRESS_PERIOD = 5;

public:
    Button(MenuCallback cb);
    ~Button();

    void updateStatus(bool pressed);
    void tick();

    bool pressed() const;
    uint32_t pressTime() const;

private:
    void _sendRepeat(uint32_t period);

    bool _pressed;
    uint32_t _pressTime;
    uint32_t _prevSendRepeatTime;
    MenuCallback _cb;
};

} // namespace Menu

#endif
