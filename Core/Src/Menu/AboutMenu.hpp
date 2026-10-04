#ifndef __MENU_ABOUTMENU_HPP__
#define __MENU_ABOUTMENU_HPP__

#include "Menu.hpp"

namespace Menu {

class AboutMenu : public Menu {
public:
    AboutMenu(CommonStuff const& common);
    virtual ~AboutMenu();

    virtual char const* name() override;
    virtual void up(Button::Event e) override;
    virtual void down(Button::Event e) override;
    virtual void right(Button::Event e) override;

    virtual void _display() override;
};

} // namespace Menu

#endif
