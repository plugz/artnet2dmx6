#ifndef __ARTNET2DMX6_H__
#define __ARTNET2DMX6_H__

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void artnet2dmx6_init_beforehal();
void artnet2dmx6_init_afterhal();
void artnet2dmx6_init_sysinit();
void artnet2dmx6_init_beforeloop();
void artnet2dmx6_tick();

// uint8_t[4], uint8_t[4], uint8_t[4]
void artnet2dmx6_lwip_init_bfor(uint8_t* ip, uint8_t* mask, uint8_t* gateway);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
