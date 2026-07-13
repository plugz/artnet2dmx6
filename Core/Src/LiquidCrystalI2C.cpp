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
    std::fill(_currentHardDisplay, _currentHardDisplay + sizeof(_currentHardDisplay), ' ');
    std::fill(_display, _display + sizeof(_display), ' ');

    _commandQueue = 0;
    _currentCmd = 0;
    _currentCmdStep = 0;

//    _writeIdx = COLS * ROWS;
}

void LiquidCrystalI2C::begin() {
    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands.
    //delay(50);
    HAL_Delay(50);

    // Now we pull both RS and R/W low to begin commands
    _expanderWrite(_backlightval);    // reset expanderand turn backlight off (Bit 8 =1)
    //delay(1000);
    HAL_Delay(6); // wait min 4.1ms ?

    //put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    _write4bits(0x03 << 4);
    //delayMicroseconds(4500); // wait min 4.1ms
    HAL_Delay(6);

    // second try
    _write4bits(0x03 << 4);
//    delayMicroseconds(4500); // wait min 4.1ms
    HAL_Delay(6);

    // third go!
    _write4bits(0x03 << 4);
//    delayMicroseconds(150);
    HAL_Delay(2);

    // finally, set to 4-bit interface
    _write4bits(0x02 << 4);

    // set # lines, font size, etc.
    _command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    _command(LCD_DISPLAYCONTROL | _displaycontrol);

    // clear it off
    _command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
    //delayMicroseconds(2000);  // this command takes a long time!
    HAL_Delay(3);

    // set the entry mode
    _command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);

    _command(LCD_RETURNHOME);  // set cursor position to zero
    //delayMicroseconds(2000);  // this command takes a long time!
    HAL_Delay(3);
}

void LiquidCrystalI2C::setCursor(uint8_t col, uint8_t row, bool now){
    if (now) {
        _commandQueue |= CMD_MASK(CMD_MOVECURSOR);
        _currentCol = col;
        _currentRow = row;
    }
    else {
        _commandQueue |= CMD_MASK(CMD_MOVECURSORAFTER);
        _moveCursorAfterCol = col;
        _moveCursorAfterRow = row;
    }
}

// Turn on and off the blinking cursor
void LiquidCrystalI2C::setCursorBlink(bool blink) {
    if (blink)
        _displaycontrol |= LCD_BLINKON;
    else
        _displaycontrol &= ~LCD_BLINKON;
    _commandQueue |= CMD_MASK(CMD_DISPLAYCTRL);
}

// Turns the underline cursor on/off
void LiquidCrystalI2C::setCursorDisplay(bool cursor) {
    if (cursor)
        _displaycontrol |= LCD_CURSORON;
    else
        _displaycontrol &= ~LCD_CURSORON;
    _commandQueue |= CMD_MASK(CMD_DISPLAYCTRL);
}

// Turn the (optional) backlight off/on
void LiquidCrystalI2C::setBacklight(bool backlight) {
    _backlightval = backlight ? LCD_BACKLIGHT : LCD_NOBACKLIGHT;
    _commandQueue |= CMD_MASK(CMD_BACKLIGHT);
}

void LiquidCrystalI2C::printLine(uint8_t line, char const* str) {
    unsigned int len = std::min(strlen(str), (size_t)COLS);
    std::copy(str, str + len, _display + line * COLS);
    std::fill(_display + line * COLS + len, _display + line * COLS + COLS, ' ');

    if (_currentRow >= line) {
        // when a write command has started, cursor has to move forward right away
        _currentCol = 0;
        _currentRow = line;
    }

    _commandQueue |= CMD_MASK(CMD_WRITE);
}

void LiquidCrystalI2C::tick() {
    using Cmd = void (LiquidCrystalI2C::*)(void);
    static Cmd constexpr cmds[CMD_COUNT] =
    {
        &LiquidCrystalI2C::_moveCursor,
        &LiquidCrystalI2C::_backlight,
        &LiquidCrystalI2C::_displayCtrl,
        &LiquidCrystalI2C::_write,
        &LiquidCrystalI2C::_moveCursorAfter,
    };
    for (uint32_t cmd = 1; cmd < CMD_LAST; ++cmd) {
        if (_currentCmd == cmd) {
            (this->*cmds[cmd - 1])();
            return;
        }
    }
    for (uint32_t cmd = 1; cmd < CMD_LAST; ++cmd) {
        if (_commandQueue & CMD_MASK(cmd)) {
            (this->*cmds[cmd - 1])();
            return;
        }
    }
}

//void LiquidCrystalI2C::print(char const* str, bool wrap) {
//    unsigned int charCount = wrap ? strlen(str) : std::min(strlen(str), (size_t)(COLS - _currentCol));
//    for (unsigned int i = 0; i < charCount; ++i) {
//        write(str[i]);
//    }
//}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystalI2C::_command(uint8_t value) {
    _send(value, 0);
}

inline void LiquidCrystalI2C::_write() {
    auto idx = _currentRow * COLS + _currentCol;
    if (_currentCmd != CMD_WRITE) {
        if (idx == 39) {
            idx = 39;
        }
        if (_currentHardDisplay[idx] != _display[idx]) {
            if (_currentHardCol != _currentCol || _currentHardRow != _currentRow) {
                _moveCursor();
                return;
            }
            _currentCmd = CMD_WRITE;
            _currentCmdStep = 0;
            _writeByte = _display[idx];
            return;
        }
        else {
            _advanceCursor(false);
        }
    }
    else if (_currentCmdStep == 0) {
        // step 0 : send high val & enable pulse
        uint8_t highnib= (_writeByte&0xf0) | Rs;
        highnib |= _backlightval;
        highnib |= En;
        if (!_i2cSend(highnib, 2)) // pulse TIMER : 450ns
            return;
        ++_currentCmdStep;
    }
    else if (_currentCmdStep == 1) {
        // step 2 : pulse end
        uint8_t highnib= (_writeByte&0xf0) | Rs;
        highnib |= _backlightval;
        if (!_i2cSend(highnib, 2)) // TIMER : 37us
            return;
        ++_currentCmdStep;
    }
    else if (_currentCmdStep == 2) {
        // step 3 : lownib val send & pulse
        uint8_t lownib=((_writeByte<<4)&0xf0) | Rs;
        lownib |= En;
        lownib |= _backlightval;
        if (!_i2cSend(lownib, 2))
            return;
        ++_currentCmdStep;
    }
    else { // _currentCmdStep == 3
        // step 5 : nownib pulse end
        uint8_t lownib=((_writeByte<<4)&0xf0) | Rs;
        lownib |= _backlightval;
        if (!_i2cSend(lownib, 2)) // TIMER : 450ns
            return;
        ++_currentCmdStep;

        _currentHardDisplay[idx] = _writeByte;
        _currentCmd = 0;
        _currentCmdStep = 0;
        _advanceCursor(true);
    }

    if (idx + 1 == sizeof(_display)) {
        if (_currentCmd == CMD_WRITE)
            _currentCmd = 0;
        _commandQueue &= ~CMD_MASK(CMD_WRITE);
    }
}

void LiquidCrystalI2C::_moveCursor() {
    int constexpr row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (_currentCmd == 0) {
        _currentCmd = CMD_MOVECURSOR;
        _currentCmdStep = 0;
        _commandQueue &= ~CMD_MASK(CMD_MOVECURSOR);

        _currentCmdCol = _currentCol;
        _currentCmdRow = _currentRow;
    }

    uint8_t cmd = LCD_SETDDRAMADDR | (_currentCmdCol + row_offsets[_currentCmdRow]);
    uint8_t highnib = (cmd & 0xf0) | _backlightval;
    uint8_t lownib = ((cmd << 4) & 0xf0) | _backlightval;

    if (_currentCmdStep == 0) {
        // step 0 : send high val & enable pulse
        if (!_i2cSend(highnib | En, 2)) // pulse TIMER : 450ns
            return;
    }
    else if (_currentCmdStep == 1) {
        // step 2 : pulse end
        if (!_i2cSend(highnib, 2)) // TIMER : 37us
            return;
    }
    else if (_currentCmdStep == 2) {
        // step 3 : lownib val send & pulse
        if (!_i2cSend(lownib | En, 2))
            return;
    }
    else { // _currentCmdStep == 3
        // step 5 : nownib pulse end
        if (!_i2cSend(lownib, 2)) // TIMER : 450ns
            return;

        _currentHardRow = _currentCmdRow;
        _currentHardCol = _currentCmdCol;

        _currentCmd = 0;
        _currentCmdStep = 0;
        return;
    }

    ++_currentCmdStep;
}

void LiquidCrystalI2C::_moveCursorAfter() {
    int constexpr row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (_currentCmd == 0) {
        _currentCmd = CMD_MOVECURSORAFTER;
        _currentCmdStep = 0;
        _commandQueue &= ~CMD_MASK(CMD_MOVECURSORAFTER);

        _currentCmdCol = _moveCursorAfterCol;
        _currentCmdRow = _moveCursorAfterRow;
    }

    uint8_t cmd = LCD_SETDDRAMADDR | (_currentCmdCol + row_offsets[_currentCmdRow]);
    uint8_t highnib = (cmd & 0xf0) | _backlightval;
    uint8_t lownib = ((cmd << 4) & 0xf0) | _backlightval;

    if (_currentCmdStep == 0) {
        // step 0 : send high val & enable pulse
        if (!_i2cSend(highnib | En, 2)) // pulse TIMER : 450ns
            return;
    }
    else if (_currentCmdStep == 1) {
        // step 2 : pulse end
        if (!_i2cSend(highnib, 2)) // TIMER : 37us
            return;
    }
    else if (_currentCmdStep == 2) {
        // step 3 : lownib val send & pulse
        if (!_i2cSend(lownib | En, 2))
            return;
    }
    else { // _currentCmdStep == 3
        // step 5 : nownib pulse end
        if (!_i2cSend(lownib, 2)) // TIMER : 450ns
            return;

        _currentRow = _currentCmdRow;
        _currentCol = _currentCmdCol;
        _currentHardRow = _currentCmdRow;
        _currentHardCol = _currentCmdCol;

        _currentCmd = 0;
        _currentCmdStep = 0;
        return;
    }

    ++_currentCmdStep;
}

void LiquidCrystalI2C::_backlight() {
    if (_currentCmd == 0) {
        _currentCmd = CMD_BACKLIGHT;
        _currentCmdStep = 0;
        _commandQueue &= ~CMD_MASK(CMD_BACKLIGHT);
    }

    if (_i2cSend(_backlightval, 0))
        return;
    _currentCmd = 0;
}

void LiquidCrystalI2C::_displayCtrl() {
    if (_currentCmd == 0) {
        _currentCmd = CMD_DISPLAYCTRL;
        _currentCmdStep = 0;
        _commandQueue &= ~CMD_MASK(CMD_DISPLAYCTRL);
    }


    uint8_t cmd = LCD_DISPLAYCONTROL | _displaycontrol;
    uint8_t highnib = (cmd & 0xf0) | _backlightval;
    uint8_t lownib = ((cmd << 4) & 0xf0) | _backlightval;

    if (_currentCmdStep == 0) {
        // step 0 : send high val & enable pulse
        if (!_i2cSend(highnib | En, 2)) // pulse TIMER : 450ns
            return;
    }
    else if (_currentCmdStep == 1) {
        // step 2 : pulse end
        if (!_i2cSend(highnib, 2)) // TIMER : 37us
            return;
    }
    else if (_currentCmdStep == 2) {
        // step 3 : lownib val send & pulse
        if (!_i2cSend(lownib | En, 2))
            return;
    }
    else { // _currentCmdStep == 3
        // step 5 : nownib pulse end
        if (!_i2cSend(lownib, 2)) // TIMER : 450ns
            return;

        _currentCmd = 0;
        _currentCmdStep = 0;
        return;
    }

    ++_currentCmdStep;
}

void LiquidCrystalI2C::_advanceCursor(bool doHard) {
    auto newCol = _currentCol + 1;
    _currentRow = (_currentRow + newCol / COLS) % ROWS;
    _currentCol = newCol % COLS;

    if (doHard) {
        if (_currentRow != _currentHardRow) {
            setCursor(_currentCol, _currentRow, true);
        }
        else {
            _currentHardRow = _currentRow;
            _currentHardCol = _currentCol;
        }
    }
}


/************ low level data pushing commands **********/

// write either command or data
void LiquidCrystalI2C::_send(uint8_t value, uint8_t mode) {
    uint8_t highnib=value&0xf0;
    uint8_t lownib=(value<<4)&0xf0;
    _write4bits((highnib)|mode);
    _write4bits((lownib)|mode);
}

void LiquidCrystalI2C::_write4bits(uint8_t value) {
    _expanderWrite(value);
    _pulseEnable(value);
}

void LiquidCrystalI2C::_expanderWrite(uint8_t _data){
    _data |= _backlightval;
    HAL_I2C_Master_Transmit(_i2cHandle, _addr, &_data, 1, 1000);
}

void LiquidCrystalI2C::_pulseEnable(uint8_t _data){
    _expanderWrite(_data | En);    // En high
    //delayMicroseconds(1);        // enable pulse must be >450ns
    HAL_Delay(2);

    _expanderWrite(_data & ~En);    // En low
    //delayMicroseconds(50);        // commands need > 37us to settle
    HAL_Delay(2);
}

bool LiquidCrystalI2C::_i2cSend(uint8_t val, uint32_t delay) {
    if (HAL_GetTick() < _prevCmdFinishTime)
        return false;
    HAL_I2C_Master_Transmit(_i2cHandle, _addr, &val, 1, 1000);
    _prevCmdFinishTime = HAL_GetTick() + delay;
    return true;
}
