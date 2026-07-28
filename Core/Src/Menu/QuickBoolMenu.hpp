#ifndef __MENU_QUICKBOOLMENU_HPP__
#define __MENU_QUICKBOOLMENU_HPP__

#include "GenericMenu.hpp"

namespace Menu {

class QuickBoolMenu : public GenericMenu<bool> {
public:
    QuickBoolMenu(CommonStuff const& common);
    virtual ~QuickBoolMenu();

    virtual char const* name() override;

protected:
    virtual void _enable() override;

protected:
    char _name[19];
};

} // namespace Menu

#endif
