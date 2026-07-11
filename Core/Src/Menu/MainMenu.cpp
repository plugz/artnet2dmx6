#include "MainMenu.hpp"

namespace Menu {

MainMenu::MainMenu(CommonStuff const& common)
    : MainMenuContainer(common) {
}

MainMenu::~MainMenu() {}

char const* MainMenu::name() { return "coucou"; }

} // namespace Menu
