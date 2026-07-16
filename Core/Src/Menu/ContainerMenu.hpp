#ifndef __MENU_CONTAINERMENU_HPP__
#define __MENU_CONTAINERMENU_HPP__

#include "Menu.hpp"

#include <array>
#include <tuple>

namespace Menu {

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
};

} // namespace Menu

#endif
