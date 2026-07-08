#include "Artnet2Dmx6.hpp"

// C part

void artnet2dmx6_init() {
    Artnet2Dmx6::init();
}

// CPP part

static Artnet2Dmx6* s_instance = nullptr;

void Artnet2Dmx6::init() {
    static Artnet2Dmx6 _instance;
    s_instance = &_instance;
}

