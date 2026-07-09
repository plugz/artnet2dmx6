#include "Artnet2Dmx6.hpp"

#include "ExternalBounce.hpp"
#include "InputOutputMCPSPI.hpp"

#include "spi.h"

static Artnet2Dmx6 artnet2dmx6;
static uint32_t currentTime;
static ExternalBounce buttons[7];
static InputOutputMCPSPI mcp;

// C part

void artnet2dmx6_init_beforehal() {
    artnet2dmx6.init_beforehal();
}

void artnet2dmx6_init_afterhal() {
    artnet2dmx6.init_afterhal();
}

void artnet2dmx6_init_sysinit() {
    artnet2dmx6.init_sysinit();
}

void artnet2dmx6_init_beforeloop() {
    artnet2dmx6.init_beforeloop();
}

void artnet2dmx6_tick() {
    artnet2dmx6.tick();
}

// CPP part

// mcp

static void mcp_setup() {
    mcp.setup(SPI2_NSS2_GPIO_GPIO_Port, SPI2_NSS2_GPIO_Pin, hspi2, 0x00);
}

static void mcp_reset() {
}

static void mcp_tick() {
    mcp.tick();
}

// buttons

static void buttons_setup() {
    for (auto& button : buttons) {
        button.setInterval(5);
    }
}

static void buttons_reset() {
}

static void buttons_tick() {
    unsigned int i = 0;
    for (auto& button: buttons) {
        if (button.update(!mcp.getCurrentValue(i), currentTime)) { // '!' because 0 low means pressed
            // TODO connect to menu
            // handleButton(i, button.read());
        }
        ++i;
    }
}

void Artnet2Dmx6::init_beforehal() {
}

void Artnet2Dmx6::init_afterhal() {
}

void Artnet2Dmx6::init_sysinit() {
}

void Artnet2Dmx6::init_beforeloop() {
    mcp_setup();
    buttons_setup();
}

void Artnet2Dmx6::tick() {
    currentTime = HAL_GetTick();
    mcp_tick();
    buttons_tick();
}
