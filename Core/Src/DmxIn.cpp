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
    if (!_nextPackets[0]) {
        _npcb(_nextPackets, _nextPackets + 1);
    }

    if (!_readyToSendPackets[0])
        return;

    _prcb(_readyToSendPackets[0], _readyToSendPackets[1]);
    for (auto& p : _readyToSendPackets)
        p = {};
}
