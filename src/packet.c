#include "packet.h"
#include "string.h"

void Packet_setBeacon(Packet_t *packet, uint8_t type, uint8_t version, 
        uint8_t CRC, uint8_t CRCH, uint8_t RXS_Max, uint8_t TXA_Max, uint8_t TXS_Max){
    PacketHeader_t *header = &packet->header;

    BEACON_t beacon = header->Beacon;

    beacon.type = type;
    beacon.version = version;
    beacon.CRC = CRC;
    beacon.CRCH = CRCH;
    beacon.RXS_Max = RXS_Max;
    beacon.TXA_Max = TXA_Max;
    beacon.TXS_Max = TXS_Max;
}

