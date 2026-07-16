#ifndef __MENU_UNIVERSESMENU_HPP__
#define __MENU_UNIVERSESMENU_HPP__

#include "ContainerMenu.hpp"
#include "UniverseMenu.hpp"

namespace Menu {

class UniversesMenu
    : public ContainerMenu<UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu>
{
public:
    UniversesMenu(CommonStuff const& common);
    virtual ~UniversesMenu();

    void init(Menu* parent);

    virtual char const* name() override;
};

} // namespace Menu

#endif
