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
        _mcp.setValues(1);
        _status = 0;
    }
    else {
        _mcp.setValues(0);
    }
}

void StatusLeds::setArtnetIn() {
    _status |= 0b10000000;
}

void StatusLeds::setArtnetOut() {
    _status |= 0b01000000;
}

void StatusLeds::setDmxIn() {
    _status |= 0b00100000;
}

void StatusLeds::setDmxOut(unsigned int idx) {
    _status |= 1 << idx;
}
