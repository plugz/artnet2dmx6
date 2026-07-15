#ifndef __MENU_STATSMENU_HPP__
#define __MENU_STATSMENU_HPP__

#include "Chrono.hpp"
#include "Menu.hpp"

namespace Menu {

class StatsMenu : public Menu {
public:
    enum { REFRESH_PERIOD_MS = 200 };
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
    MsTimer _refreshTimer{Milliseconds{REFRESH_PERIOD_MS}};
};

} // namespace Menu

#endif
