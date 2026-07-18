#include "LiquidCrystalI2C.hpp"

#include "Stats.hpp"

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

    _printCol = COLS;
    _printRow = ROWS;

    _cmdQueue = 0;
    _currentCmd = 0;
    _currentCmdStep = 0;

    _i2cTimer.reset(Chrono::Microseconds{0});

//    _writeIdx = COLS * ROWS;
}

void LiquidCrystalI2C::begin() {
    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands.
    Chrono::delay(Chrono::Milliseconds{50});

    // Now we pull both RS and R/W low to begin commands
    _expanderWrite(_backlightval);    // reset expanderand turn backlight off (Bit 8 =1)
    Chrono::delay(Chrono::Microseconds{4200}); // wait min 4.1ms ?

    //put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    _write4bits(0x03 << 4);
    Chrono::delay(Chrono::Microseconds{4200}); // wait min 4.1ms ?

    // second try
    _write4bits(0x03 << 4);
    Chrono::delay(Chrono::Microseconds{4200}); // wait min 4.1ms ?

    // third go!
    _write4bits(0x03 << 4);
    Chrono::delay(Chrono::Microseconds{160});

    // finally, set to 4-bit interface
    _write4bits(0x02 << 4);

    // set # lines, font size, etc.
    _command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    _command(LCD_DISPLAYCONTROL | _displaycontrol);

    // clear it off
    _command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
    Chrono::delay(Chrono::Microseconds{2100});  // this command takes a long time!

    // set the entry mode
    _command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);

    _command(LCD_RETURNHOME);  // set cursor position to zero
    Chrono::delay(Chrono::Microseconds{2100});  // this command takes a long time!
}

void LiquidCrystalI2C::setCursor(uint8_t col, uint8_t row, bool now){
    if (now) {
        _cmdQueue |= CMD_MASK(CMD_MOVECURSOR);
        _currentCol = col;
        _currentRow = row;
    }
    else {
        _cmdQueue |= CMD_MASK(CMD_MOVECURSORAFTER);
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
    _cmdQueue |= CMD_MASK(CMD_DISPLAYCTRL);
}

// Turns the underline cursor on/off
void LiquidCrystalI2C::setCursorDisplay(bool cursor) {
    if (cursor)
        _displaycontrol |= LCD_CURSORON;
    else
        _displaycontrol &= ~LCD_CURSORON;
    _cmdQueue |= CMD_MASK(CMD_DISPLAYCTRL);
}

// Turn the (optional) backlight off/on
void LiquidCrystalI2C::setBacklight(bool backlight) {
    _backlightval = backlight ? LCD_BACKLIGHT : LCD_NOBACKLIGHT;
    _cmdQueue |= CMD_MASK(CMD_BACKLIGHT);
}

void LiquidCrystalI2C::printLine(uint8_t line, char const* str) {
    unsigned int len = std::min(strlen(str), (size_t)COLS);
    std::copy(str, str + len, _display + line * COLS);
    std::fill(_display + line * COLS + len, _display + line * COLS + COLS, ' ');

    if (_printRow >= line) {
        _printRow = line;
        _printCol = 0;
    }

    _cmdQueue |= CMD_MASK(CMD_PRINT);
}

bool LiquidCrystalI2C::ready() const {
    return !(_cmdQueue | _currentCmd);
}

void LiquidCrystalI2C::tick() {
    using Cmd = void (LiquidCrystalI2C::*)(void);
    static Cmd constexpr cmds[CMD_COUNT] =
    {
        &LiquidCrystalI2C::_sendCmd,
        &LiquidCrystalI2C::_moveCursor,
        &LiquidCrystalI2C::_backlight,
        &LiquidCrystalI2C::_displayCtrl,
        &LiquidCrystalI2C::_write,
        &LiquidCrystalI2C::_print,
        &LiquidCrystalI2C::_moveCursorAfter,
    };
    if (_currentCmd) {
        (this->*cmds[_currentCmd - 1])();
        return;
    }
    if (_cmdQueue) {
        for (uint32_t cmd = 1; cmd < CMD_LAST; ++cmd) {
            if (_cmdQueue & CMD_MASK(cmd)) {
                (this->*cmds[cmd - 1])();
                return;
            }
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

void LiquidCrystalI2C::_command(uint8_t value) {
    _send(value, 0);
}

void LiquidCrystalI2C::_sendCmd() {
    if (_currentCmd == 0) {
        _currentCmd = CMD_SENDCMD;
        _currentCmdStep = 0;
    }

    uint8_t highnib = (_sendCmdByte & 0xf0) | _backlightval | _sendCmdByteMode;
    uint8_t lownib = ((_sendCmdByte << 4) & 0xf0) | _backlightval | _sendCmdByteMode;

    if (_currentCmdStep == 0) {
        // step 0 : send high val & enable pulse
        if (!_i2cSend(highnib | En, Chrono::Microseconds{2})) // pulse TIMER : 450ns
            return;
    }
    else if (_currentCmdStep == 1) {
        // step 2 : pulse end
        if (!_i2cSend(highnib, Chrono::Microseconds{38})) // TIMER : 37us
            return;
    }
    else if (_currentCmdStep == 2) {
        // step 3 : lownib val send & pulse
        if (!_i2cSend(lownib | En, Chrono::Microseconds{2}))
            return;
    }
    else { // _currentCmdStep == 3
        // step 5 : nownib pulse end
        if (!_i2cSend(lownib, Chrono::Microseconds{38}))
            return;

        _currentCmd = 0;
        _currentCmdStep = 0;
        if (_sendCmdEnd)
            _sendCmdEnd();
        return;
    }

    ++_currentCmdStep;
}

void LiquidCrystalI2C::_print() {
    _cmdQueue &= ~CMD_MASK(CMD_PRINT);
    _cmdQueue |= CMD_MASK(CMD_WRITE);
    _currentCol = _printCol;
    _currentRow = _printRow;
}

void LiquidCrystalI2C::_write() {
    _writeIdx = _currentRow * COLS + _currentCol;

    while ((_writeIdx + 1 != sizeof(_display)) && (_currentHardDisplay[_writeIdx] == _display[_writeIdx])) {
        _advanceCursor(false);
        _writeIdx = _currentRow * COLS + _currentCol;
    }

    if (_writeIdx + 1 == sizeof(_display)) {
        _cmdQueue &= ~CMD_MASK(CMD_WRITE);
        return;
    }

    if (_currentHardCol != _currentCol || _currentHardRow != _currentRow) {
        _moveCursor();
        return;
    }

    _writeByte = _display[_writeIdx];
    _sendCmdByte = _writeByte;
    _sendCmdByteMode = Rs;
    _sendCmdEnd = [this](){
        _currentHardDisplay[_writeIdx] = _writeByte;
        _advanceCursor(!(_cmdQueue & CMD_MASK(CMD_MOVECURSOR)));
        if (_writeIdx + 1 == sizeof(_display)) {
            _cmdQueue &= ~CMD_MASK(CMD_WRITE);
        }
    };

    _sendCmd();
}

void LiquidCrystalI2C::_moveCursor() {
    int constexpr row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

    _cmdQueue &= ~CMD_MASK(CMD_MOVECURSOR);

    _currentCmdCol = _currentCol;
    _currentCmdRow = _currentRow;

    _sendCmdByte = LCD_SETDDRAMADDR | (_currentCmdCol + row_offsets[_currentCmdRow]);
    _sendCmdByteMode = 0;
    _sendCmdEnd = [this](){
        _currentRow = _currentCmdRow;
        _currentCol = _currentCmdCol;
        _currentHardRow = _currentCmdRow;
        _currentHardCol = _currentCmdCol;

        // prolly have to write from here
        _cmdQueue |= CMD_MASK(CMD_WRITE);
    };

    _sendCmd();
}

void LiquidCrystalI2C::_moveCursorAfter() {
    int constexpr row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

    _cmdQueue &= ~CMD_MASK(CMD_MOVECURSORAFTER);

    _currentCmdCol = _moveCursorAfterCol;
    _currentCmdRow = _moveCursorAfterRow;

    _sendCmdByte = LCD_SETDDRAMADDR | (_currentCmdCol + row_offsets[_currentCmdRow]);
    _sendCmdByteMode = 0;
    _sendCmdEnd = [this](){
        _currentRow = _currentCmdRow;
        _currentCol = _currentCmdCol;
        _currentHardRow = _currentCmdRow;
        _currentHardCol = _currentCmdCol;
    };

    _sendCmd();
}

void LiquidCrystalI2C::_backlight() {
    _cmdQueue &= ~CMD_MASK(CMD_BACKLIGHT);
    _currentCmd = CMD_BACKLIGHT;

    if (!_i2cSend(_backlightval, Chrono::Microseconds{0}))
        return;

    _currentCmd = 0;
}

void LiquidCrystalI2C::_displayCtrl() {
    _cmdQueue &= ~CMD_MASK(CMD_DISPLAYCTRL);

    _sendCmdByte = LCD_DISPLAYCONTROL | _displaycontrol;
    _sendCmdByteMode = 0;
    _sendCmdEnd = {};

    _sendCmd();
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
    Chrono::delay(Chrono::Microseconds{2}); // enable pulse must be >450ns

    _expanderWrite(_data & ~En);    // En low
    Chrono::delay(Chrono::Microseconds{40}); // commands need > 37us to settle
}

bool LiquidCrystalI2C::_i2cSend(uint8_t val, Chrono::Microseconds delay) {
    if (!(_transmitDone && _i2cTimer.done()))
        return false;

    _transmitDone = false;
    _i2cByte = val;
    HAL_I2C_Master_Transmit_IT(_i2cHandle, _addr, &_i2cByte, 1);
    _i2cTimer.reset(delay);
    return true;
}
