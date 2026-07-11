#ifndef __MENU_MENU_HPP__
#define __MENU_MENU_HPP__

#include "Button.hpp"

#include <array>
#include <tuple>

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

template<typename... TSubmenus>
class ContainerMenu : public Menu {
public:
    ContainerMenu(CommonStuff const& commonStuff)
        : Menu(commonStuff)
        , _subMenusTuple(TSubmenus(commonStuff)...)
    {
    }

    void init(Menu* parent) {
        Menu::init(parent);
        // fill array for dynamic subMenu access
        int idx = 0;
        std::apply([&](auto&... subMenu){
                (((_subMenus[idx++] = &subMenu), (subMenu.init(this))), ...);
            }, _subMenusTuple);
    }

    virtual void up(Button::Event e) override;
    virtual void down(Button::Event e) override;
    virtual void right(Button::Event e) override;

protected:
    virtual void _display() override;

protected:
    unsigned int _currentMenuIdx = 0;

    std::tuple<TSubmenus...> _subMenusTuple;
    std::array<Menu*, sizeof...(TSubmenus)> _subMenus;
    //Menu* _subMenusTuple[sizeof...(TSubmenus)];
};

} // namespace Menu

#endif
