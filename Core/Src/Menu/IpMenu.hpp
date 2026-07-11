#ifndef __MENU_IPMENU_HPP__
#define __MENU_IPMENU_HPP__

#include "Menu.hpp"

namespace Menu {

class IpMenu : public Menu {
public:
    IpMenu(CommonStuff const& common);
    virtual ~IpMenu();

    virtual char const* name() override;

    virtual void up(Button::Event e) override;
    virtual void down(Button::Event e) override;
    virtual void right(Button::Event e) override;

protected:
    virtual void _enable() override;
    virtual void _disable() override;
    virtual void _applyConfig() override;
    void _display();
    void _displayIp();
    void _displaySubnet();
    void _moveCursor();

protected:
    unsigned int _editIdx;
    uint8_t _ipParts[4];
    unsigned int _subnet;
};

} // namespace Menu

#endif
