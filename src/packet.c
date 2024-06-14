#include "packet.h"

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
