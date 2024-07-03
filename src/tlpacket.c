#include "tlpacket.h"
#include "string.h"



void TLPacket_BEACONDecode(BEACON_t *beacon, uint8_t *buf){
    //TODO
}

void TLPacket_BEACONSet(BEACON_t *beacon, uint8_t version, uint8_t CRC, uint8_t RXS_Max, 
        uint8_t TXS_Max, uint8_t TXA_Max, uint8_t CRCH){

    beacon->type = PKTTYPE_BEACON;
    beacon->version = version;
    beacon->CRC = CRC;
    beacon->CRCH = CRCH;
    beacon->RXS_Max = RXS_Max;
    beacon->TXA_Max = TXA_Max;
    beacon->TXS_Max = TXS_Max;
}

void TLPacket_PINGSet(PING_t *ping, uint8_t C, uint8_t N, uint8_t LLID, uint16_t Number, uint8_t CRCH){
    ping->type = PKTTYPE_PING;
    ping->C = C;
    ping->N = N;
    ping->LLID = LLID;
    ping->Number = Number;
    ping->CRCH = CRCH;
}



TLPacket_t *TLPacket_Create(TLPacketHeader_t *header, int type, uint8_t *payload, int payload_size){
    TLPacket_t *packet = calloc(1, sizeof(TLPacket_t));
    packet->header = *header;
    packet->type = type;
    packet->Payload = payload;
    packet->Payload_size = payload_size;
    packet->CRC = 0;

    return packet;
}

void TLPacket_Free(TLPacket_t *packet){
    free(packet);
}


/*
 * Returns 0 if packet is not of error type or the error code, otherwise
 */
int TLPacket_checkError(uint8_t *PKTbuf){
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
uint8_t TLPacket_getType(uint8_t *PKTbuf){
    uint32_t *pHeader = (uint32_t*) PKTbuf;
    int type = ((*pHeader) & 0x0f);   
    return type;
}


static bool TLPacket_checkBadCRCH(uint8_t *PKTbuf, int size){
#ifdef CRC_CHECK
    uint16_t crch = (PKTbuf[3] & 0xf0) >> 4;
    if(0){
        return true;
    }
#endif
    return false;
}

/*
 * Check for bad header (incorrect CRCH) and invalid packet
 * type. TODO: chech payload length
 */
bool TLPacket_checkBadPacket(uint8_t *PKTbuf){
    int rv = 0;
    //Chech type
    uint8_t type = TLPacket_getType(PKTbuf);
    switch(type){
        case PKTTYPE_BEACON:
        case PKTTYPE_PING:
        case PKTTYPE_ERROR:
        case PKTTYPE_REQUEST_ASYNC:
        case PKTTYPE_RESPONSE:
            break;
        default:
            rv = -1;
            goto out;
    }

    if(TLPacket_checkBadCRCH(PKTbuf, 6)){
        rv = -1;
        goto out;
    }

out:
    return rv;
}

