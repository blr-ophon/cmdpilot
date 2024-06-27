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
    ping->C = C;
    ping->N = N;
    ping->LLID = LLID;
    ping->Number = Number;
    ping->CRCH = CRCH;
}


/*
 * Returns 0 if packet is not of error type or the error code, otherwise
 */
int Packet_checkError(uint8_t *PKTbuf){
    int rv = 0;

    uint32_t *pHeader = (uint32_t*) PKTbuf;
    int type = ((*pHeader) & 0x0f);   

    if(type == PKTTYPE_ERROR){
        rv = ((*pHeader) & 0x0000ff00) >> 8;   
    }
    return rv;
}


/*
 * Return packet type
 */
uint8_t Packet_getType(uint8_t *PKTbuf){
    uint32_t *pHeader = (uint32_t*) PKTbuf;
    int type = ((*pHeader) & 0x0f);   
    return type;
}
