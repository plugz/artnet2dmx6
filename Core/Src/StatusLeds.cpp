#include "StatusLeds.hpp"

#include "InputOutputMCPSPI.hpp"

StatusLeds::StatusLeds(InputOutputMCPSPI& mcp) : _mcp(mcp) {
}

StatusLeds::~StatusLeds() {
}

void StatusLeds::init() {
    _timer.reset(Chrono::Milliseconds(100));
}

void StatusLeds::tick() {
    if (!_timer.done())
        return;
    _timer.reset();

    if (_step++ % 2) {
        _mcp.setValues(~_status);
        _status = 0;
    }
    else {
        _mcp.setValues(0xff);
    }
}

void StatusLeds::setArtnetIn() {
    _status |= 0b00000001;
}

void StatusLeds::setArtnetOut() {
    _status |= 0b00000010;
}

void StatusLeds::setDmxIn() {
    _status |= 0b00000100;
}

void StatusLeds::setDmxOut(unsigned int idx) {
    _status |= 0b10000000 >> (4 - idx);
}
