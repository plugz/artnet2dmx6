#include "artnet2dmx6.h"

#include "Config.hpp"
#include "ExternalBounce.hpp"
#include "InputOutputMCPSPI.hpp"
#include "LiquidCrystalI2C.hpp"
#include "M95640R.hpp"
#include "Menu/MainMenu.hpp"

#include "i2c.h"
#include "spi.h"

#include <cstdio>

static uint32_t currentTime;

static M95640R eeprom;
static Config config{&eeprom};

static LiquidCrystalI2C screen;
static Menu::MainMenu mainMenu{{&screen, &config}};

static InputOutputMCPSPI mcp;
static ExternalBounce buttons[7];
static Menu::Button menuButtons[4] = {{&Menu::Menu::up}, {&Menu::Menu::down}, {&Menu::Menu::left}, {&Menu::Menu::right}};

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
    //screen.tick(&sentI2cOrSpi, &screenDone, currentTime);
}

//menu
static void menu_setup() {
    mainMenu.init(nullptr);
    mainMenu.enable();
}

//static void menu_reset() {
//}

static void menu_tick() {
}

// mcp

static void mcp_setup() {
    HAL_GPIO_WritePin(MCP_RESET_GPIO_GPIO_Port, MCP_RESET_GPIO_Pin, GPIO_PIN_SET);
    // HAL_Delay(0); // datasheet says 0ns from resetHIGH to ready
    mcp.setup(SPI2_NSS2_GPIO_GPIO_Port, SPI2_NSS2_GPIO_Pin, &hspi2, 0x00);
}

//static void mcp_reset() {
//    HAL_GPIO_WritePin(MCP_RESET_GPIO_GPIO_Port, MCP_RESET_GPIO_Pin, GPIO_PIN_RESET);
//    HAL_Delay(1); // datasheet says 1us minimum reset pulse width
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
    for (auto& button: buttons) {
        if (button.update(!mcp.getCurrentValue(i), currentTime)) { // '!' because 0 means pressed
            if (i < 4)
                menuButtons[i].updateStatus(button.read());
        }
        ++i;
    }

    for (auto& menuButton: menuButtons) {
        menuButton.tick();
    }
}

// main

void artnet2dmx6_init_beforehal() {
//    printf("fuck you\n");
}

void artnet2dmx6_init_afterhal() {
}

void artnet2dmx6_init_sysinit() {
}

//extern "C" {
//    extern void initialise_monitor_handles(void);
//}

void artnet2dmx6_init_beforeloop() {
    // enable secondary 5V
    HAL_GPIO_WritePin(PWR_5V_EN_GPIO_GPIO_Port, PWR_5V_EN_GPIO_Pin, GPIO_PIN_SET);
    HAL_Delay(250);

    eeprom_setup();
    config_setup();

    screen_setup();
    menu_setup();

    mcp_setup();
    buttons_setup();

 //   initialise_monitor_handles();
}

void artnet2dmx6_tick() {
    currentTime = HAL_GetTick();

    eeprom_tick();
    config_tick();

    screen_tick();
    menu_tick();

    mcp_tick();
    buttons_tick();

//    static uint32_t prevTime = 0;
//    static int i = 0;
//    if (currentTime > prevTime + 2000) {
////        printf("fuuuuuck\n");
////        screen.printline((0 + i) % 4, "fuck");
////        screen.printline((1 + i) % 4, "this");
////        screen.printline((2 + i) % 4, "shit");
////        screen.printline((3 + i) % 4, "!");
////        screen.setBacklight(i % 2);
//        ++i;
//        prevTime = currentTime;
//    }
}
