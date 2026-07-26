#ifndef __MENU_DMXOUTUNIVERSEMENU_HPP__
#define __MENU_DMXOUTUNIVERSEMENU_HPP__

#include "GenericMenu.hpp"

namespace Menu {

class DmxOutUniverseMenu : public GenericMenu<uint16_t> {
public:
    DmxOutUniverseMenu(CommonStuff const& common);
    virtual ~DmxOutUniverseMenu();

    void initDmxOutUniverseMenu(uint8_t idx);

private:
    uint8_t _idx;
};

} // namespace Menu

#endif
