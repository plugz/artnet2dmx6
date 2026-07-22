#include "DmxIn.hpp"

#include "Packet.hpp"

DmxIn::DmxIn() {
    _bufferPtrs[0] = _buffers[0].data();
    _bufferPtrs[1] = _buffers[1].data();
}

DmxIn::~DmxIn() {}

void DmxIn::init(PacketCallback cb) {
    _callback = cb;
}

static void freeNothing(void*) {}

void DmxIn::tick() {
    if (!_mustSendBytes)
        return;

    unsigned int packetToSendBufferIdx = (_currentBufferIdx + 1) % 2;

    if (_callback) {
        Packet p{std::shared_ptr<uint8_t*>{&_bufferPtrs[packetToSendBufferIdx], freeNothing}, uint16_t(_mustSendBytes + 17), false};
        _callback(p);
    }

    _mustSendBytes = 0;
}
