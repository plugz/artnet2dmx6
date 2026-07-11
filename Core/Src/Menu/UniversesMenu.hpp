#ifndef __MENU_UNIVERSES_MENU_HPP__
#define __MENU_UNIVERSES_MENU_HPP__

#include "UniverseMenu.hpp"

namespace Menu {

class UniversesMenu
    : public ContainerMenu<UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu>
{
    UniversesMenu(CommonStuff const& common);
    virtual ~UniversesMenu();

    void init(Menu* parent);
};

} // namespace Menu

#endif
