#ifndef __MENU_SCREENOFFMENU_HPP__
#define __MENU_SCREENOFFMENU_HPP__

#include "Menu.hpp"

namespace Menu {

class ScreenOffMenu : public Menu {
public:
    ScreenOffMenu(CommonStuff const& common);
    virtual ~ScreenOffMenu();

    virtual char const* name() override;
    virtual void up(Button::Event e) override;
    virtual void down(Button::Event e) override;
    virtual void right(Button::Event e) override;

    virtual void _enable() override;
    virtual void _disable() override;
    virtual void _display() override;
};

} // namespace Menu

#endif
