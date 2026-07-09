#ifndef __ARTNET2DMX6_H__
#define __ARTNET2DMX6_H__

#ifdef __cplusplus
extern "C" {
#endif

void artnet2dmx6_init_beforehal();
void artnet2dmx6_init_afterhal();
void artnet2dmx6_init_sysinit();
void artnet2dmx6_init_beforeloop();
void artnet2dmx6_tick();

#ifdef __cplusplus
} // extern "C"
#endif

#endif
