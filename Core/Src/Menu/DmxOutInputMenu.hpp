#ifndef __MENU_DMXOUTINPUTMENU_HPP__
#define __MENU_DMXOUTINPUTMENU_HPP__

#include "QuickBoolMenu.hpp"

namespace Menu {

class DmxOutInputMenu : public QuickBoolMenu {
public:
    DmxOutInputMenu(CommonStuff const& common);
    virtual ~DmxOutInputMenu();

    virtual char const* name() override;

    void initDmxOutInputMenu(uint8_t idx);

protected:
    uint8_t _idx;
};

} // namespace Menu

#endif
