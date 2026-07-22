#include "DmxOutsMenu.hpp"

#include "ContainerMenu.ipp"

namespace Menu {

DmxOutsMenu::DmxOutsMenu(CommonStuff const& common) : DmxOutsMenuContainer(common) {
}

DmxOutsMenu::~DmxOutsMenu() {
}

void DmxOutsMenu::init(Menu* parent) {
    DmxOutsMenuContainer::init(parent);

    uint8_t idx = 0;
    std::apply([&](auto&... subMenu){
            (subMenu.initDmxOutMenu(idx++), ...);
        }, _subMenusTuple);
}

char const* DmxOutsMenu::name() {
    return "DmxOuts";
}

};
