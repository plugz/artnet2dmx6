#ifndef __MENU_BUTTON_HPP__
#define __MENU_BUTTON_HPP__

#include "Chrono.hpp"

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

    static constexpr Milliseconds LONG_PRESS_TIME{500};
    static constexpr Milliseconds LONG_LONG_PRESS_TIME{LONG_PRESS_TIME * 4};

    static constexpr Milliseconds LONG_PRESS_REPEAT_PERIOD{50};
    static constexpr Milliseconds LONG_LONG_PRESS_REPEAT_PERIOD{5};

public:
    Button(MenuCallback cb);
    ~Button();

    void updateStatus(bool pressed);
    void tick();

    bool pressed() const;

private:
    void _sendRepeat(Milliseconds period);

    bool _pressed;
    MsTimer _longPressTimer;
    MsTimer _longPressRepeatTimer;
    MenuCallback _cb;
};

} // namespace Menu

#endif
