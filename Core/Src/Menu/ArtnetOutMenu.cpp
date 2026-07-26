#include "ArtnetOutMenu.hpp"

#include "Config.hpp"
#include "ContainerMenu.ipp"

namespace Menu {

ArtnetOutMenu::ArtnetOutMenu(CommonStuff const& common)
    : ArtnetOutMenuContainer(common)
{
}

ArtnetOutMenu::~ArtnetOutMenu() {
}

char const* ArtnetOutMenu::name() {
    return "Artnet Output";
}

void ArtnetOutMenu::init(Menu* parent) {
    ArtnetOutMenuContainer::init(parent);

    // enable
    std::get<0>(_subMenusTuple).setConfig({
            .min = false,
            .max = true,
            .name = "Enable",
            .inName = "Enable Artnet Out",
            .desc = "",
            .getValueCallback = [this]() -> bool { return _common.config->artnetOutEnable(); },
            .setValueCallback = [this](bool val) -> void { _common.config->setArtnetOutEnable(val); }
            });

    std::get<ArtnetOutUniverseMenu>(_subMenusTuple).setConfig({
            .min = 0,
            .max = 0x7fff,
            .name = "Universe",
            .inName = "Artnet Universe",
            .desc = "for Artnet Out",
            .getValueCallback = [this]() -> uint16_t { return _common.config->artnetOutUniverse(); },
            .setValueCallback = [this](uint16_t val) -> void { _common.config->setArtnetOutUniverse(val); }
            });

    // manual target ip
    std::get<2>(_subMenusTuple).setConfig({
            .min = false,
            .max = true,
            .name = "Manual Target IP",
            .inName = "Manual IP",
            .desc = "for Artnet Out",
            .getValueCallback = [this]() -> bool { return _common.config->artnetOutManualTargetIp(); },
            .setValueCallback = [this](bool val) -> void { _common.config->setArtnetOutManualTargetIp(val); }
            });

//    _config.min = 0;
//    _config.max = 0x7fff;
//    snprintf(_config.name, sizeof(_config.name), "Output Universe");
//    snprintf(_config.inName, sizeof(_config.desc), "Artnet Universe");
//    snprintf(_config.desc, sizeof(_config.inName), "for Artnet Out");
//    _config.getValueCallback = [this]() -> uint16_t { return _common.config->artnetOutUniverse(); };
//    _config.setValueCallback = [this](uint16_t val) -> void { _common.config->setArtnetOutUniverse(val); };

}

void ArtnetOutMenu::_enable() {
    unsigned int maxMenuIdx = _subMenus.size() - 1;
    if (!_common.config->artnetOutEnable())
        maxMenuIdx = 0;
    else if (!_common.config->artnetOutManualTargetIp())
        maxMenuIdx -= 1;

    _currentMenuIdx = std::min(_currentMenuIdx, maxMenuIdx);

    ArtnetOutMenuContainer::_enable();
}

void ArtnetOutMenu::_display() {
    _common.screen->printLine(0, name());

    unsigned int maxMenuIdx = _subMenus.size() - 1;
    if (!_common.config->artnetOutEnable())
        maxMenuIdx = 0;
    else if (!_common.config->artnetOutManualTargetIp())
        maxMenuIdx -= 1;
    unsigned int maxMenuCount = maxMenuIdx + 1;

    // _currentMenuIdx %= _subMenus.size() - sizeReduce;

    char line[LINE_BUFF_SIZE];
    {
        if (_currentMenuIdx != 0) {
            auto idx = (_currentMenuIdx + (maxMenuIdx)) % (maxMenuCount);
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
        if (_currentMenuIdx != maxMenuIdx) {
            auto idx = (_currentMenuIdx + 1) % (maxMenuCount);
            snprintf(line, sizeof(line), "  %s", _subMenus[idx]->name());
        }
        else {
            line[0] = '\0';
        }
        _common.screen->printLine(3, line);
    }
}

} // namespace Menu

