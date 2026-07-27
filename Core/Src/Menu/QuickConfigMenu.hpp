#ifndef __MENU_QUICKCONFIGMENU_HPP__
#define __MENU_QUICKCONFIGMENU_HPP__

#include "ContainerMenu.hpp"
#include "GenericMenu.hpp"

namespace Menu {

class QuickConfigApplyMenu : public GenericMenu<bool> {
public:
    QuickConfigApplyMenu(CommonStuff const& commonStuff);
    virtual ~QuickConfigApplyMenu();

    void initQuickConfigApplyMenu(char const* name, std::function<void(bool const&)> setValueCallback);
};

// splitter, artnet2dmx, factory reset

using QuickConfigContainerMenu = ContainerMenu<QuickConfigApplyMenu, QuickConfigApplyMenu, QuickConfigApplyMenu>;

class QuickConfigMenu : public QuickConfigContainerMenu {
public:
    QuickConfigMenu(CommonStuff const& commonStuff);
    virtual ~QuickConfigMenu();

    virtual char const* name() override;

    void init(Menu* parent);
};

} // namespace Menu

#endif
