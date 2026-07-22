#ifndef __MENU_DMXOUTINPUTMENU_HPP__
#define __MENU_DMXOUTINPUTMENU_HPP__

#include "GenericMenu.hpp"

namespace Menu {

class DmxOutInputMenu : public GenericMenu<bool> {
public:
    DmxOutInputMenu(CommonStuff const& common);
    virtual ~DmxOutInputMenu();

    void initDmxOutInputMenu(uint8_t idx);

protected:
    virtual void _displayValue() override;

private:
    uint8_t _idx;
};

} // namespace Menu

#endif
