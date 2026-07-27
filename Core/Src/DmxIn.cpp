#include "DmxIn.hpp"

#include "Packet.hpp"

DmxIn::DmxIn() {
}

DmxIn::~DmxIn() {}

void DmxIn::init(NewPacketCallback npcb, PacketReadyCallback prcb) {
    _npcb = npcb;
    _prcb = prcb;
}

void DmxIn::tick() {
    if (!_nextPacket)
        _nextPacket = _npcb();

    if (!_readyToSendPacket)
        return;

    _prcb(_readyToSendPacket);
    _readyToSendPacket = {};
}
