#include "LiquidCrystalI2C.hpp"

#include "i2c.h"

#include <algorithm>
#include <cstring>

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

void LiquidCrystalI2C::init(I2C_HandleTypeDef* i2cHandle, uint8_t lcd_addr)
{
    _i2cHandle = i2cHandle;
    _addr = lcd_addr;
    _backlightval = LCD_BACKLIGHT;
    _currentCol = 0;
    _currentRow = 0;
    _currentHardCol = 0;
    _currentHardRow = 0;
    for (auto& current : _currentDisplay)
        current = ' ';
}

void LiquidCrystalI2C::begin() {
    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands.
    //delay(50);
    HAL_Delay(50);

    // Now we pull both RS and R/W low to begin commands
    expanderWrite(_backlightval);    // reset expanderand turn backlight off (Bit 8 =1)
    //delay(1000);
    HAL_Delay(6); // wait min 4.1ms ?

    //put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03 << 4);
    //delayMicroseconds(4500); // wait min 4.1ms
    HAL_Delay(6);

    // second try
    write4bits(0x03 << 4);
//    delayMicroseconds(4500); // wait min 4.1ms
    HAL_Delay(6);

    // third go!
    write4bits(0x03 << 4);
//    delayMicroseconds(150);
    HAL_Delay(2);

    // finally, set to 4-bit interface
    write4bits(0x02 << 4);

    // set # lines, font size, etc.
    command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    command(LCD_DISPLAYCONTROL | _displaycontrol);

    // clear it off
    command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
    //delayMicroseconds(2000);  // this command takes a long time!
    HAL_Delay(3);

    // set the entry mode
    command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);

    command(LCD_RETURNHOME);  // set cursor position to zero
    //delayMicroseconds(2000);  // this command takes a long time!
    HAL_Delay(3);
}

void LiquidCrystalI2C::setCursor(uint8_t col, uint8_t row, bool doIt){
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row > ROWS) {
        row = ROWS - 1;    // we count rows starting w/0
    }
    if (doIt) {
        command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
        _currentHardCol = col;
        _currentHardRow = row;
    }
    _currentCol = col;
    _currentRow = row;
}

// Turn on and off the blinking cursor
void LiquidCrystalI2C::setCursorBlink(bool blink) {
    if (blink)
        _displaycontrol |= LCD_BLINKON;
    else
        _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystalI2C::setCursorDisplay(bool cursor) {
    if (cursor)
        _displaycontrol |= LCD_CURSORON;
    else
        _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn the (optional) backlight off/on
void LiquidCrystalI2C::setBacklight(bool backlight) {
    _backlightval = backlight ? LCD_BACKLIGHT : LCD_NOBACKLIGHT;
    expanderWrite(0);
}

void LiquidCrystalI2C::printLine(uint8_t line, char const* str) {
    unsigned int len = std::min(strlen(str), (size_t)COLS);
    setCursor(0, line, false);
    unsigned int i = 0;
    for (; i < len; ++i) {
        write(str[i]);
    }
    for (; i < COLS; ++i) {
        write(' ');
    }
}

void LiquidCrystalI2C::print(char const* str, bool wrap) {
    unsigned int charCount = wrap ? strlen(str) : std::min(strlen(str), (size_t)(COLS - _currentCol));
    for (unsigned int i = 0; i < charCount; ++i) {
        write(str[i]);
    }
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystalI2C::command(uint8_t value) {
    send(value, 0);
}

inline void LiquidCrystalI2C::write(uint8_t value) {
    auto idx = _currentRow * COLS + _currentCol;
    if (_currentDisplay[idx] != value) {
        if (_currentHardCol != _currentCol || _currentHardRow != _currentRow)
            setCursor(_currentCol, _currentRow, true);
        send(value, Rs);
        _currentDisplay[idx] = value;
        advanceCursor(true);
    }
    else {
        advanceCursor(false);
    }
}

void LiquidCrystalI2C::advanceCursor(bool doHard) {
    auto newCol = _currentCol + 1;
    _currentRow = (_currentRow + newCol / COLS) % ROWS;
    _currentCol = newCol % COLS;

    if (doHard) {
        if (_currentRow != _currentHardRow) {
            setCursor(_currentCol, _currentRow, true);
        }
        _currentHardRow = _currentRow;
        _currentHardCol = _currentCol;
    }
}


/************ low level data pushing commands **********/

// write either command or data
void LiquidCrystalI2C::send(uint8_t value, uint8_t mode) {
    uint8_t highnib=value&0xf0;
    uint8_t lownib=(value<<4)&0xf0;
    write4bits((highnib)|mode);
    write4bits((lownib)|mode);
}

void LiquidCrystalI2C::write4bits(uint8_t value) {
    expanderWrite(value);
    pulseEnable(value);
}

void LiquidCrystalI2C::expanderWrite(uint8_t _data){
    _data |= _backlightval;
    HAL_I2C_Master_Transmit(_i2cHandle, _addr, &_data, 1, 1000);
}

void LiquidCrystalI2C::pulseEnable(uint8_t _data){
    expanderWrite(_data | En);    // En high
    //delayMicroseconds(1);        // enable pulse must be >450ns
    HAL_Delay(2);

    expanderWrite(_data & ~En);    // En low
    //delayMicroseconds(50);        // commands need > 37us to settle
    HAL_Delay(2);
}
