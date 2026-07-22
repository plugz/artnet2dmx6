#ifndef __MENU_DMXOUTSMENU_HPP__
#define __MENU_DMXOUTSMENU_HPP__

#include "ContainerMenu.hpp"
#include "DmxOutMenu.hpp"

namespace Menu {

using DmxOutsMenuContainer = ContainerMenu<DmxOutMenu, DmxOutMenu, DmxOutMenu, DmxOutMenu, DmxOutMenu>;

class DmxOutsMenu : public DmxOutsMenuContainer {
public:
    DmxOutsMenu(CommonStuff const& common);
    virtual ~DmxOutsMenu();

    void init(Menu* parent);

    virtual char const* name() override;
};

} // namespace Menu

#endif
