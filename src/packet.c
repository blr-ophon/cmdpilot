#include "packet.h"
#include "string.h"

void Beacon_decode(BEACON_t *beacon, uint8_t *buf){
    memcpy(beacon, buf, sizeof(BEACON_t));
}

void Beacon_set(BEACON_t *beacon, uint8_t version, uint8_t CRC, uint8_t RXS_Max, 
        uint8_t TXS_Max, uint8_t TXA_Max, uint8_t CRCH){

    beacon->type = PKTTYPE_BEACON;
    beacon->version = version;
    beacon->CRC = CRC;
    beacon->CRCH = CRCH;
    beacon->RXS_Max = RXS_Max;
    beacon->TXA_Max = TXA_Max;
    beacon->TXS_Max = TXS_Max;
}

void Ping_set(PING_t *ping, uint8_t C, uint8_t N, uint8_t LLID, uint16_t Number, uint8_t CRCH){
    ping->type = PKTTYPE_PING;
    ping->C1 = C;
    ping->C2 = C;
    ping->N1 = N;
    ping->N2 = N;
    ping->LLID = LLID;
    ping->Number = Number;
    ping->CRCH = CRCH;
}


/*
 * Returns 0 if packet is not of error type or the error code, otherwise
 */
int Packet_checkError(uint8_t *PKTbuf){
    int rv = 0;
    ERROR_t errorHeader;
    memcpy(&errorHeader, PKTbuf, sizeof(PacketHeader_t));

    if(errorHeader.type == PKTTYPE_ERROR){
        rv = errorHeader.ErrorCode1;
    }
    return rv;
}

/*
 * Return packet type
 */
uint8_t Packet_getType(uint8_t *PKTbuf){
    uint8_t type = PKTbuf[0] & 0x0f; //first 4 bits of header
    return type;
}
