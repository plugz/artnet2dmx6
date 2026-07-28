#include "QuickBoolMenu.hpp"

namespace Menu {

QuickBoolMenu::QuickBoolMenu(CommonStuff const& common) : GenericMenu<bool>(common) {
}

QuickBoolMenu::~QuickBoolMenu() {
}

char const* QuickBoolMenu::name() {
    snprintf(_name, sizeof(_name), "%s \xa5%s\xa5", _config.name, _config.getValueCallback() ? "Y" : "N");
    return _name;
}

void QuickBoolMenu::_enable() {
    _value = !_config.getValueCallback();
    applyConfig();
}

} // namespace Menu
