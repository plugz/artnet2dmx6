#ifndef __LIQUIDCRYSTALI2C_HPP__
#define __LIQUIDCRYSTALI2C_HPP__

#include "i2c.h"

#include <cstdint>

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

    void setCursor(uint8_t col, uint8_t row, bool doIt = true);

    // \0 terminated string
    void printLine(uint8_t line, char const* str);
    //void print(char const* str, bool wrap = false);

    void tick();

private:
    void _write();
    void _moveCursor();
    void _moveCursorUser();
    void _backlight();
    void _displayCtrl();
    void _advanceCursor(bool doHard);
    void _command(uint8_t);
    void _send(uint8_t, uint8_t);
    void _write4bits(uint8_t);
    void _expanderWrite(uint8_t);
    void _pulseEnable(uint8_t);

    I2C_HandleTypeDef* _i2cHandle;
    uint8_t _addr;

    uint8_t _displaycontrol;
    uint8_t _backlightval;

    uint8_t _currentHardCol; // actual col in screen
    uint8_t _currentHardRow; // actual row in screen
    uint8_t _currentCol;
    uint8_t _currentRow;
    uint8_t _currentHardDisplay[COLS * ROWS];
    uint8_t _display[COLS * ROWS];

    enum {
        CMD_MOVECURSOR = 1,
        CMD_BACKLIGHT,
        CMD_DISPLAYCTRL,
        CMD_WRITE,
        CMD_MOVECURSORUSER,
        CMD_COUNT,
    };

#define CMD_MASK(cmd) (1 << cmd)

    uint32_t _commandQueue;
    uint32_t _currentCmd;
    uint32_t _currentCmdStep;

    uint32_t _currentWriteIdx;

    uint8_t _moveCursorUserRow;
    uint8_t _moveCursorUserCol;
};

#endif
