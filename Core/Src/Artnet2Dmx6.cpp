#include "Artnet2Dmx6.hpp"

#include "ExternalBounce.hpp"
#include "InputOutputMCPSPI.hpp"
#include "LiquidCrystalI2C.hpp"

#include "i2c.h"
#include "spi.h"

#include <cstdio>

static Artnet2Dmx6 artnet2dmx6;
static uint32_t currentTime;
static ExternalBounce buttons[7];
static InputOutputMCPSPI mcp;
static LiquidCrystalI2C screen;

// C part

int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
      ITM_SendChar(*ptr++);
  }
  return len;
}

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
    HAL_GPIO_WritePin(MCP_RESET_GPIO_GPIO_Port, MCP_RESET_GPIO_Pin, GPIO_PIN_SET);
    HAL_Delay(10); // TODO check datasheet
    mcp.setup(SPI2_NSS2_GPIO_GPIO_Port, SPI2_NSS2_GPIO_Pin, &hspi2, 0x00);
}

static void mcp_reset() {
    HAL_GPIO_WritePin(MCP_RESET_GPIO_GPIO_Port, MCP_RESET_GPIO_Pin, GPIO_PIN_RESET);
    HAL_Delay(10); // TODO check datasheet
    mcp_setup();
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
            //screen.setCursor(0, i);
            //screen.write(button.read() ? '1' : '0');
        }
        ++i;
    }
}

// screen
static void screen_setup() {
    // i2c: 8bit address, not 7bit
    screen.init(&hi2c1, 0b01000000, 20, 4);
    screen.begin();
    screen.setCursor(0, 0);
    screen.write('a');
    screen.setCursor(0, 1);
    screen.write('b');
    screen.setCursor(0, 2);
    screen.write('c');
    screen.setCursor(0, 3);
    screen.write('d');
}

static void screen_reset() {
}

static void screen_tick() {
    bool sentI2cOrSpi = false;
    bool screenDone = false;
//    screen.tick(&sentI2cOrSpi, &screenDone, currentTime);
//    if (screenDone)
//        serialOut.sendScreenDone();
}

void Artnet2Dmx6::init_beforehal() {
//    printf("fuck you\n");
}

void Artnet2Dmx6::init_afterhal() {
}

void Artnet2Dmx6::init_sysinit() {
}

extern "C" {
    extern void initialise_monitor_handles(void);
}

void Artnet2Dmx6::init_beforeloop() {
    // enable secondary 5V
    HAL_GPIO_WritePin(PWR_5V_EN_GPIO_GPIO_Port, PWR_5V_EN_GPIO_Pin, GPIO_PIN_SET);
    HAL_Delay(250);

    mcp_setup();
    buttons_setup();
    screen_setup();

 //   initialise_monitor_handles();
}

void Artnet2Dmx6::tick() {
    currentTime = HAL_GetTick();
    mcp_tick();
    buttons_tick();
    screen_tick();

    static uint32_t prevTime = 0;
    static int i = 0;
    if (currentTime > prevTime + 2000) {
//        printf("fuuuuuck\n");
//        screen.printline((0 + i) % 4, "fuck");
//        screen.printline((1 + i) % 4, "this");
//        screen.printline((2 + i) % 4, "shit");
//        screen.printline((3 + i) % 4, "!");
        screen.setBacklight(i % 2);
        ++i;
        prevTime = currentTime;
    }
}
