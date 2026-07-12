#ifndef __MENU_UNIVERSE_MENU_HPP__
#define __MENU_UNIVERSE_MENU_HPP__

#include "GenericMenu.hpp"

namespace Menu {

class UniverseMenu : public GenericMenu<uint16_t> {
public:
    UniverseMenu(CommonStuff const& common);
    virtual ~UniverseMenu();

    void initUniverseMenu(uint8_t idx);

private:
    uint8_t _idx;
};

} // namespace Menu

#endif
