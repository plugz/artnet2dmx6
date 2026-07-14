#ifndef __MENU_STATSMENU_HPP__
#define __MENU_STATSMENU_HPP__

#include "Menu.hpp"

namespace Menu {

class StatsMenu : public Menu {
public:
    StatsMenu(CommonStuff const& common);
    virtual ~StatsMenu();

    virtual char const* name() override;
    virtual void up(Button::Event e) override;
    virtual void down(Button::Event e) override;
    virtual void right(Button::Event e) override;

    virtual void tick() override;

protected:
    virtual void _enable() override;
    virtual void _disable() override;
    virtual void _display() override;

private:
    uint32_t _prevRefreshTime = 0;
};

} // namespace Menu

#endif
