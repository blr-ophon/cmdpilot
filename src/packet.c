#include "packet.h"
#include "string.h"

int Packet_CRCCheck(Packet_t *packet){
    if(0){
        return -BADCRC;
    }
    return 0;
}

int Packet_CRCHCheck(PacketHeader_t *header){
    uint32_t *pHeader = (uint32_t*) header;
    if(0){
        return -BADCRC;
    }
    return 0;
}

int Packet_setBeacon(PacketHeader_t *header, uint8_t type, uint8_t version, 
        uint8_t CRC, uint8_t CRCH, uint8_t RXS_Max, uint8_t TXA_Max, uint8_t TXS_Max){
    BEACON_t beacon = header->Beacon;

    beacon.type = type;
    beacon.version = version;
    beacon.CRC = CRC;
    beacon.CRCH = CRCH;
    beacon.RXS_Max = RXS_Max;
    beacon.TXA_Max = TXA_Max;
    beacon.TXS_Max = TXS_Max;
}

int Packet_Serialize(Packet_t *packet, uint8_t *buf){
    int rv = 0;

    int i = 0;

    /* Header + payload + CRC */
    int len = sizeof(PacketHeader_t) + packet->Payload_size + 2;
    memcpy(&buf[i], &packet->header, sizeof(PacketHeader_t));
    i += sizeof(PacketHeader_t);
    memcpy(&buf[i], packet->Payload, packet->Payload_size);
    i += packet->Payload_size;
    memcpy(&buf[i], &packet->CRC, 2);
     
out:
    return rv;
}
