#ifndef __MENU_MENU_HPP__
#define __MENU_MENU_HPP__

#include "Button.hpp"

class LiquidCrystalI2C;
class Config;

namespace Menu {

struct CommonStuff {
    LiquidCrystalI2C* screen;
    Config* config;
};

class Menu {
public:
    Menu(CommonStuff const& commonStuff) : _common(commonStuff), _parent(nullptr) {}
    virtual ~Menu();

    void init(Menu* parent) {
        _parent = parent;
    }

    void enable();
    static Menu* current();

    virtual char const* name() = 0;
    virtual void up(Button::Event e) = 0;
    virtual void down(Button::Event e) = 0;
    virtual void left(Button::Event e);  // back, WARNING only override for locking a screen
    virtual void right(Button::Event e) = 0;

    virtual void tick();

    void applyConfig(bool changeMenu = true);

protected:
    virtual void _applyConfig();
    virtual void _enable();
    virtual void _disable();
    virtual void _display() = 0;

protected:
    CommonStuff _common;
    Menu* _parent;
    static Menu* _current;

};

} // namespace Menu

#endif
