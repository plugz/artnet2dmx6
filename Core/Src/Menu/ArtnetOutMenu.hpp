#ifndef __MENU_ARTNETOUTMENU_HPP__
#define __MENU_ARTNETOUTMENU_HPP__

#include "ArtnetOutIpMenu.hpp"
#include "ContainerMenu.hpp"
#include "GenericMenu.hpp"
#include "QuickBoolMenu.hpp"

namespace Menu {

using ArtnetOutEnableMenu = QuickBoolMenu;
using ArtnetOutUniverseMenu = GenericMenu<uint16_t>;
using ArtnetOutManualTargetIpMenu = QuickBoolMenu;

using ArtnetOutMenuContainer = ContainerMenu<ArtnetOutEnableMenu, ArtnetOutUniverseMenu, ArtnetOutManualTargetIpMenu, ArtnetOutIpMenu>;

class ArtnetOutMenu : public ArtnetOutMenuContainer {
public:
    ArtnetOutMenu(CommonStuff const& common);
    virtual ~ArtnetOutMenu();

    virtual char const* name() override;

    void init(Menu* parent);

protected:
    virtual void _enable() override;
    virtual void _display() override;

private:
    uint8_t _idx;
};

} // namespace Menu


#endif
