#include "serialize.h"

#define MIN(a,b) ( ((a) < (b)) ? (a) : (b) )

int Serialize_Packet(Packet_t *packet, uint8_t *buf, int buf_size){
    int rv = 0;

    int i = 0;

    //TODO: Use MIN to assert boundaries

    /* Header + payload + CRC */
    memcpy(&buf[i], &packet->header, sizeof(PacketHeader_t));
    i += sizeof(PacketHeader_t);
    memcpy(&buf[i], packet->Payload, packet->Payload_size);
    i += packet->Payload_size;
    memcpy(&buf[i], &packet->CRC, 2);
     
out:
    return rv;
}

