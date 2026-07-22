#ifndef __MENU_GENERICINTMENU_HPP__
#define __MENU_GENERICINTMENU_HPP__

#include "Menu.hpp"

#include <functional>

namespace Menu {

template <typename T> class GenericMenu : public Menu {
public:
    struct Conf {
        T min;
        T max;
        char name[16] = "INT";
        char inName[16] = "EDIT INT";
        char desc[16] = "";
        std::function<T(void)> getValueCallback = nullptr;
        std::function<void(T const&)> setValueCallback = nullptr;
    };

public:
    GenericMenu(CommonStuff const& common, Conf const& c);
    virtual ~GenericMenu();

    void setConfig(Conf const& c);

    virtual char const* name() override;

    virtual void up(Button::Event e) override;
    virtual void down(Button::Event e) override;
    virtual void right(Button::Event e) override;

protected:
    virtual void _enable() override;
    virtual void _disable() override;
    virtual void _applyConfig() override;
    virtual void _display() override;

    virtual void _displayValue();

protected:
    Conf _config;
    T _value;
};

using BoolMenu = GenericMenu<bool>;
using IntMenu = GenericMenu<int>;
using UintMenu = GenericMenu<unsigned int>;
using Int64Menu = GenericMenu<int64_t>;
using Uint64Menu = GenericMenu<uint64_t>;

} // namespace Menu

#endif
