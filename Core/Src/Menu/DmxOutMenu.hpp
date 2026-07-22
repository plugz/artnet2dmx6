#ifndef __MENU_DMXOUTMENU_HPP__
#define __MENU_DMXOUTMENU_HPP__

#include "ContainerMenu.hpp"
#include "DmxOutInputMenu.hpp"
#include "UniverseMenu.hpp"

namespace Menu {

using DmxOutMenuContainer = ContainerMenu<DmxOutInputMenu, UniverseMenu>;

class DmxOutMenu : public DmxOutMenuContainer {
public:
    DmxOutMenu(CommonStuff const& common);
    virtual ~DmxOutMenu();

    void initDmxOutMenu(uint8_t idx);

    virtual char const* name() override;

protected:
    virtual void _enable() override;
    virtual void _display() override;

private:
    uint8_t _idx;
    char _name[16];
};

} // namespace Menu

#endif
