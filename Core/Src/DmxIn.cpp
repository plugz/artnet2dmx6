#include "DmxIn.hpp"

#include "Packet.hpp"

DmxIn::DmxIn() {
    for (unsigned int i = 0; i < std::size(_buffers); ++i) {
        _bufferPtrs[i] = _buffers[i].data();
    }
}

DmxIn::~DmxIn() {}

void DmxIn::init(PacketCallback cb) {
    _callback = cb;
}

static void freeNothing(void*) {}

void DmxIn::tick() {
    if (!_mustSendBytes)
        return;

    // send previous packet (previous from the one that is being written in)
    unsigned int packetToSendBufferIdx = (_currentBufferIdx + (std::size(_buffers) - 1)) % std::size(_buffers);

    if (_callback) {
        Packet p{std::shared_ptr<uint8_t*>{&_bufferPtrs[packetToSendBufferIdx], freeNothing}, uint16_t(_mustSendBytes + 17), false};
        _callback(p);
    }

    _mustSendBytes = 0;
}
