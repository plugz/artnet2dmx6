#ifndef __LIQUIDCRYSTALI2C_HPP__
#define __LIQUIDCRYSTALI2C_HPP__

#include "Chrono.hpp"

#include "i2c.h"

#include <cstdint>
#include <cstdio>
#include <functional>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

/**
 * This is the driver for the Liquid Crystal LCD displays that use the I2C bus.
 *
 * After creating an instance of this class, first call begin() before anything else.
 * The backlight is on by default, since that is the most likely operating mode in
 * most cases.
 */
class LiquidCrystalI2C {
public:
    enum {
        COLS = 20,
        ROWS = 4,
    };

public:
    /**
     * Constructor
     *
     * @param lcd_addr    I2C slave address of the LCD display. Most likely printed on the
     *                    LCD circuit board, or look in the supplied LCD documentation.
     * @param lcd_cols    Number of columns your LCD display has.
     * @param lcd_rows    Number of rows your LCD display has.
     * @param charsize    The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
     */
    void init(I2C_HandleTypeDef* i2cHandle, uint8_t lcd_addr);

    /**
     * Set the LCD display in the correct begin state, must be called before anything else is done.
     */
    void begin();

    void setCursorBlink(bool blink);
    void setCursorDisplay(bool cursor);
    void setBacklight(bool backlight);

    void setCursor(uint8_t col, uint8_t row, bool now = true); // false -> after

    // \0 terminated string
    void printLine(uint8_t line, char const* str);

    template<typename T, typename...Ts>
    void printLine(uint8_t line, char const* str, T firstArg, Ts... nextArgs) {
        char buff[COLS + 1];
        snprintf(buff, sizeof(buff), str, firstArg, nextArgs...);
        printLine(line, buff);
    }

    //void print(char const* str, bool wrap = false);

    // return true when no command in queue
    bool ready() const;

    void tick();

private:
    void _sendCmd();
    void _moveCursor();
    void _backlight();
    void _displayCtrl();
    void _print();
    void _write();
    void _moveCursorAfter();

    void _advanceCursor(bool doHard);
    void _command(uint8_t);
    void _send(uint8_t, uint8_t);
    void _write4bits(uint8_t);
    void _expanderWrite(uint8_t);
    void _pulseEnable(uint8_t);
    bool _i2cSend(uint8_t val, Microseconds delay);

    I2C_HandleTypeDef* _i2cHandle;
    uint8_t _addr;

    uint8_t _displaycontrol;
    uint8_t _backlightval;

    uint8_t _currentHardCol; // actual col in screen
    uint8_t _currentHardRow; // actual row in screen
    uint8_t _currentHardDisplay[COLS * ROWS];

    uint8_t _currentCol;
    uint8_t _currentRow;

    uint8_t _printCol;
    uint8_t _printRow;
    uint8_t _display[COLS * ROWS];

    enum {
        CMD_SENDCMD = 1,
        CMD_MOVECURSOR,
        CMD_BACKLIGHT,
        CMD_DISPLAYCTRL,
        CMD_WRITE,
        CMD_PRINT,
        CMD_MOVECURSORAFTER,
        CMD_LAST,
    };
#define CMD_MASK(cmd) (1 << cmd)
#define CMD_COUNT (LiquidCrystalI2C::CMD_LAST - 1)

    uint32_t _cmdQueue;
    uint8_t _currentCmd;
    uint8_t _currentCmdStep;

    uint8_t _sendCmdByte;
    uint8_t _sendCmdByteMode;
    using SendCmdEndCallback = std::function<void(void)>;
    SendCmdEndCallback _sendCmdEnd;

    uint8_t _writeByte;
    uint8_t _writeIdx;

    uint8_t _currentCmdRow;
    uint8_t _currentCmdCol;

    uint8_t _moveCursorAfterRow;
    uint8_t _moveCursorAfterCol;

    UsTimer _i2cTimer{Microseconds{0}};
};

#endif
