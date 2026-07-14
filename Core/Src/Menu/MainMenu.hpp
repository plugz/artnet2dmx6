#ifndef __MENU_MAINMENU_HPP__
#define __MENU_MAINMENU_HPP__

#include "ContainerMenu.hpp"
#include "IpMenu.hpp"
#include "StatsMenu.hpp"
#include "UniversesMenu.hpp"

namespace Menu {

using MainMenuContainer = ContainerMenu<StatsMenu, IpMenu, UniversesMenu>;

class MainMenu : public MainMenuContainer
{
public:
    MainMenu(CommonStuff const& common);
    virtual ~MainMenu();

    virtual char const* name() override;
};

} // namespace Menu

#endif
