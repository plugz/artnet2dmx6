#include "UniversesMenu.hpp"

namespace Menu {

UniversesMenu::UniversesMenu(CommonStuff const& common)
    : ContainerMenu<UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu>(common)
{
}

UniversesMenu::~UniversesMenu() {
}

void UniversesMenu::init(Menu* parent) {
    ContainerMenu<UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu>::init(parent);

    uint8_t idx = 0;
    std::apply([&](auto&... subMenu){
            (subMenu.initUniverseMenu(idx++), ...);
        }, _subMenusTuple);
}

char const* UniversesMenu::name() {
    return "Artnet Universes";
}

};
