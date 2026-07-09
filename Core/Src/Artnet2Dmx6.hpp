#ifndef __ARTNET2DMX6_HPP__
#define __ARTNET2DMX6_HPP__

#include "artnet2dmx6.h"

class Artnet2Dmx6 {
public:
    void init_beforehal();
    void init_afterhal();
    void init_sysinit();
    void init_beforeloop();
    void tick();

    int i;
};

#endif
