#include "artnet2dmx6.h"

#include "Chrono.hpp"
#include "Config.hpp"
#include "ExternalBounce.hpp"
#include "InputOutputMCPSPI.hpp"
#include "LiquidCrystalI2C.hpp"
#include "M95640R.hpp"
#include "Menu/MainMenu.hpp"
#include "Stats.hpp"

#include "i2c.h"
#include "lwip.h"
#include "spi.h"
#include "udp.h"

#include <cstdio>

static M95640R eeprom;
static Config config{&eeprom};

static LiquidCrystalI2C screen;
static Menu::MainMenu mainMenu{{&screen, &config}};

static InputOutputMCPSPI mcp;
static ExternalBounce buttons[7];
static Menu::Button menuButtons[4] = {{&Menu::Menu::up}, {&Menu::Menu::down}, {&Menu::Menu::left}, {&Menu::Menu::right}};

Stats a2d6Stats;

static Chrono::MsTimer msTimer{Chrono::Milliseconds{1500}};
static Chrono::UsTimer usTimer(Chrono::Microseconds{1500000});

udp_pcb* udp;

// C part

//int _write(int file, char *ptr, int len)
//{
//  (void)file;
//  int DataIdx;
//
//  for (DataIdx = 0; DataIdx < len; DataIdx++)
//  {
//      ITM_SendChar(*ptr++);
//  }
//  return len;
//}

// CPP part

// eeprom

static void eeprom_setup() {
    eeprom.setup(SPI2_NSS1_GPIO_GPIO_Port,
                 SPI2_NSS1_GPIO_Pin,
                 &hspi2,
                 EEPROM_WRITE_GPIO_GPIO_Port,
                 EEPROM_WRITE_GPIO_Pin,
                 EEPROM_HOLD_GPIO_GPIO_Port,
                 EEPROM_HOLD_GPIO_Pin);
    eeprom.begin();
}

//Static void eeprom_reset() {
//}

static void eeprom_tick() {
}

// config

static void config_setup() {
    config.setup({}, {});
}

//Static void config_reset() {
//}

static void config_tick() {
}

// screen
static void screen_setup() {
    // i2c: 8bit address, not 7bit
    screen.init(&hi2c1, 0b01000000);
    screen.begin();

//    screen.setCursor(0, 0);
//    screen.print("0A234567890B234567890C234567890D23456789", true);
//    screen.setCursor(0, 0);
//    screen.print(" A23456789 B23456789 C23456789 D23456789", true);
//    screen.setCursor(0, 0);
//    screen.print("0A2345    0B12345   ", true);
}

//static void screen_reset() {
//}

static void screen_tick() {
    //bool sentI2cOrSpi = false;
    //bool screenDone = false;
    screen.tick();
    //screen.tick(&sentI2cOrSpi, &screenDone);
}

//menu
static void menu_setup() {
    mainMenu.init(nullptr);
    mainMenu.enable();
}

//static void menu_reset() {
//}

static void menu_tick() {
    Menu::Menu::current()->tick();
}

// mcp

static void mcp_setup() {
    HAL_GPIO_WritePin(MCP_RESET_GPIO_GPIO_Port, MCP_RESET_GPIO_Pin, GPIO_PIN_SET);
    // Chrono::delay(Chrono::Microseconds{0}); // datasheet says 0ns from resetHIGH to ready
    mcp.setup(SPI2_NSS2_GPIO_GPIO_Port, SPI2_NSS2_GPIO_Pin, &hspi2, 0x00);
}

//static void mcp_reset() {
//    HAL_GPIO_WritePin(MCP_RESET_GPIO_GPIO_Port, MCP_RESET_GPIO_Pin, GPIO_PIN_RESET);
//    Chrono::delay(Chrono::Microseconds{2}); // datasheet says 1us minimum reset pulse width
//    mcp_setup();
//}

static void mcp_tick() {
    mcp.tick();
}

// buttons

static void buttons_setup() {
    for (auto& button : buttons) {
        button.setInterval(5);
    }
}

//static void buttons_reset() {
//}

static void buttons_tick() {
    unsigned int i = 0;
    auto nowMs = Chrono::MsClock::now();
    for (auto& button: buttons) {
        if (button.update(!mcp.getCurrentValue(i), nowMs.time_since_epoch().count())) { // '!' because 0 means pressed
            if (i < 4)
                menuButtons[i].updateStatus(button.read());
        }
        ++i;
    }

    for (auto& menuButton: menuButtons) {
        menuButton.tick();
    }
}

// stats
static void stats_setup() {
}

//static void stats_reset() {
//}

static void stats_tick() {
    a2d6Stats.tick();
}

// main


void artnet2dmx6_init_beforehal() {
}

void artnet2dmx6_init_afterhal() {
}

void artnet2dmx6_init_sysinit() {
}

//extern "C" {
//    extern void initialise_monitor_handles(void);
//}

static void udp_receive_callback(
        void *arg,
        struct udp_pcb *pcb,
        struct pbuf *p,
        const ip_addr_t *addr,
        u16_t port) {
    a2d6Stats.increaseCounter(3);

    pbuf_free(p);
}


void artnet2dmx6_init_beforeloop() {
    Chrono::init();

    // enable secondary 5V
    HAL_GPIO_WritePin(PWR_5V_EN_GPIO_GPIO_Port, PWR_5V_EN_GPIO_Pin, GPIO_PIN_SET);
    Chrono::delay(Chrono::Milliseconds{250});

    eeprom_setup();
    config_setup();

    screen_setup();
    menu_setup();

    mcp_setup();
    buttons_setup();

    stats_setup();

    MX_LWIP_Init();

    udp = udp_new();
    udp_bind(udp, IP_ADDR_ANY, 4455);
    udp_recv(udp, udp_receive_callback, nullptr);

    msTimer.reset();
    usTimer.reset();

 //   initialise_monitor_handles();
}

void artnet2dmx6_tick() {
    eeprom_tick();
    config_tick();

    screen_tick();
    menu_tick();

    mcp_tick();
    buttons_tick();

    stats_tick();

    MX_LWIP_Process();

    if (msTimer.done()) {
        msTimer.advance();
        a2d6Stats.increaseCounter(0);
    }
    if (usTimer.done()) {
        usTimer.advance();
        a2d6Stats.increaseCounter(1);
    }
}

//lwip
void artnet2dmx6_lwip_init_bfor(uint8_t* ip, uint8_t* mask, uint8_t* gateway) {
    uint32_t const cfgIp = config.ip();
    ip[0] = uint8_t(cfgIp >> 24);
    ip[1] = uint8_t(cfgIp >> 16);
    ip[2] = uint8_t(cfgIp >> 8);
    ip[3] = uint8_t(cfgIp >> 0);

    uint32_t const cfgSubnet = config.subnet();
    uint32_t const subnetMask = (cfgSubnet ? ~(uint64_t(1 << (32 - cfgSubnet)) - 1) : 0);
    mask[0] = uint8_t(subnetMask >> 24);
    mask[1] = uint8_t(subnetMask >> 16);
    mask[2] = uint8_t(subnetMask >> 8);
    mask[3] = uint8_t(subnetMask >> 0);

    (void)gateway;
}
