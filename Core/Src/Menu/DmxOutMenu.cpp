#include "DmxOutMenu.hpp"

#include "Config.hpp"
#include "ContainerMenu.ipp"

namespace Menu {

DmxOutMenu::DmxOutMenu(CommonStuff const& common)
    : DmxOutMenuContainer(common)
{
}

DmxOutMenu::~DmxOutMenu() {
}

void DmxOutMenu::initDmxOutMenu(uint8_t idx) {
    _idx = idx;
    snprintf(_name, sizeof(_name), "DMX Out #%i", _idx + 1);

    std::get<DmxOutInputMenu>(_subMenusTuple).initDmxOutInputMenu(_idx);
    std::get<UniverseMenu>(_subMenusTuple).initUniverseMenu(_idx);
}

char const* DmxOutMenu::name() {
    return _name;
}

void DmxOutMenu::_enable() {
    _currentMenuIdx = 0;

    DmxOutMenuContainer::_enable();
}

void DmxOutMenu::_display() {
    _common.screen->printLine(0, name());
    unsigned int sizeReduce = _common.config->dmxOutInputDmx(_idx) ? 1 : 0;

    _currentMenuIdx %= _subMenus.size() - sizeReduce;

    char line[LINE_BUFF_SIZE];
    {
        if (_currentMenuIdx != 0) {
            auto idx = (_currentMenuIdx + (_subMenus.size() - sizeReduce - 1)) % (_subMenus.size() - sizeReduce);
            snprintf(line, sizeof(line), "  %s", _subMenus[idx]->name());
        }
        else {
            line[0] = '\0';
        }
        _common.screen->printLine(1, line);
    }
    {
        auto idx = _currentMenuIdx;
        snprintf(line, sizeof(line), "> %s", _subMenus[idx]->name());
        _common.screen->printLine(2, line);
    }
    {
        if (_currentMenuIdx != _subMenus.size() - sizeReduce - 1) {
            auto idx = (_currentMenuIdx + 1) % (_subMenus.size() - sizeReduce);
            snprintf(line, sizeof(line), "  %s", _subMenus[idx]->name());
        }
        else {
            line[0] = '\0';
        }
        _common.screen->printLine(3, line);
    }
}

} // namespace Menu
