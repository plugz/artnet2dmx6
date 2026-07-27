#include "QuickConfigMenu.hpp"

#include "Config.hpp"

namespace Menu {

QuickConfigApplyMenu::QuickConfigApplyMenu(CommonStuff const& commonStuff) : GenericMenu<bool>(commonStuff) {
    _config.min = false;
    _config.max = true;
    _config.getValueCallback = []() -> bool { return false; };
}

QuickConfigApplyMenu::~QuickConfigApplyMenu() {
}

void QuickConfigApplyMenu::initQuickConfigApplyMenu(char const* name, std::function<void(bool const&)> setValueCallback) {
    snprintf(_config.name, sizeof(_config.name), "%s", name);
    snprintf(_config.inName, sizeof(_config.name), "Apply %s ?", name);
    _config.setValueCallback = setValueCallback;
}

QuickConfigMenu::QuickConfigMenu(CommonStuff const& commonStuff)
 : QuickConfigContainerMenu(commonStuff) {
}

QuickConfigMenu::~QuickConfigMenu() {
}

char const* QuickConfigMenu::name() {
    return "Quick Config";
}

void QuickConfigMenu::init(Menu* parent) {
    QuickConfigContainerMenu::init(parent);

    // splitter
    std::get<0>(_subMenusTuple).initQuickConfigApplyMenu(
            "DMX Splitter",
            [this](bool const& value) { if (value) this->_common.config->applyConfSplitter(); }
            );

    // artnet2dmx
    std::get<1>(_subMenusTuple).initQuickConfigApplyMenu(
            "Artnet to DMX",
            [this](bool const& value) { if (value) this->_common.config->applyConfArtnetToDmx(); }
            );

    // factory reset
    std::get<2>(_subMenusTuple).initQuickConfigApplyMenu(
            "RESET ALL",
            [this](bool const& value) { if (value) this->_common.config->applyConfResetAll(); }
            );
}

} // namespace Menu
