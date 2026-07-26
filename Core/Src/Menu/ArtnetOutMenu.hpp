#ifndef __MENU_ARTNETOUTMENU_HPP__
#define __MENU_ARTNETOUTMENU_HPP__

#include "ContainerMenu.hpp"
#include "GenericMenu.hpp"
#include "ArtnetOutIpMenu.hpp"

namespace Menu {

using ArtnetOutEnableMenu = GenericMenu<bool>;
using ArtnetOutUniverseMenu = GenericMenu<uint16_t>;
using ArtnetOutManualTargetIpMenu = GenericMenu<bool>;

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
