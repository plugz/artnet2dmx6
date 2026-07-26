#ifndef __STATUSLEDS_HPP__
#define __STATUSLEDS_HPP__

#include "Chrono.hpp"

class InputOutputMCPSPI;

class StatusLeds {
public:
    StatusLeds(InputOutputMCPSPI& mcp);
    ~StatusLeds();

    void init();
    void tick();

    void setArtnetIn();
    void setArtnetOut();
    void setDmxIn();
    void setDmxOut(unsigned int idx);

private:
    InputOutputMCPSPI& _mcp;
    Chrono::MsTimer _timer;

    uint8_t _step = 0;
    uint8_t _status = 0;
};

#endif
