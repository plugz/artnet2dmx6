#ifndef __DMXIN_HPP__
#define __DMXIN_HPP__

#include <array>
#include <cstdint>

class Packet;

class DmxIn {
public:
    using PacketCallback = void(*)(Packet const&);

public:
    DmxIn();
    ~DmxIn();

    void init(PacketCallback cb);
    void tick();

    // called from interrupt : no heavy processing here
    inline void handleBreak() {
        if (_currentByteIdx == 0)
            return;

        _swapAndSend();
    }

    // called from interrupt : no heavy processing here
    inline void handleByte(uint8_t byte) {
        _currentBufferDmxData[_currentByteIdx++] = byte;
        if (_currentByteIdx < 513)
            return;

        _swapAndSend();
    }

private:
    inline void _swapAndSend() {
        _mustSendBytes = _currentByteIdx;
        _currentBufferIdx = (_currentBufferIdx + 1) % 2;
        _currentBufferDmxData = _buffers[_currentBufferIdx].data() + 17;
        _currentByteIdx = 0;
    }

private:
    // 512 +18 to have same size as artnet dmx packet
    std::array<uint8_t, 512 + 18> _buffers[2];
    uint8_t* _bufferPtrs[2];
    uint16_t _currentBufferIdx = 0;

    uint8_t* _currentBufferDmxData = nullptr;
    uint16_t _currentByteIdx = 0;

    PacketCallback _callback = nullptr;

    uint16_t _mustSendBytes = 0;
};

#endif
