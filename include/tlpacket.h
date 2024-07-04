#ifndef PACKET_H
#define PACKET_H

/*
 * Functions for packet encoding, decoding and CRC check
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define BADCRC  1

typedef enum{
    PKTTYPE_BEACON               =   5,
    PKTTYPE_PING                 =   6,
    PKTTYPE_ERROR                =   15,
    PKTTYPE_REQUEST_ASYNC        =   9,
    PKTTYPE_RESPONSE             =   10,
}Packet_type;




/*
 * Types of TLPacket Headers
 */

typedef struct{
    uint8_t type:4;
    uint8_t version:3;
    uint8_t CRC:1;
    uint8_t RXS_Max:6;
    uint8_t TXS_Max:7;
    uint8_t TXA_Max:7;
    uint8_t CRCH:4;
}__attribute__((packed))BEACON_t;

typedef struct{
    uint8_t type:4;
    uint8_t C:1;
    uint8_t N:1;
    uint8_t LLID:4;
    uint16_t Number;
    uint8_t CRCH:4;
}PING_t;

typedef struct{
    uint8_t type:4;
    uint8_t ErrorCode;
    uint8_t CRCH:4;
}ERROR_t;

typedef struct{
    uint8_t type:4;
    uint16_t payloadLength:13;
    uint8_t CRCH:4;
}ASYNC_t;

typedef struct{
    uint8_t type:4;
    uint16_t payloadLength:13;
    uint8_t CRCH:4;
}RESPONSE_t;

typedef struct{
    uint8_t type:4;
    uint32_t HeaderContent:24;
    uint8_t CRCH:4;
}ANY_t;





/*
 * TLPacket structs
 */

typedef union{
    BEACON_t Beacon;
    PING_t Ping;
    ERROR_t Error;
    ASYNC_t Async;
    RESPONSE_t Response;
    ANY_t Any;
}TLPacketHeader_t;


typedef struct{
    TLPacketHeader_t header;
    int type;
    uint8_t *Payload;
    uint16_t CRC;               
    uint8_t Payload_size;
}TLPacket_t;






/*
 * Set Header structs
 */
void TLPacket_BEACONSet(BEACON_t *beacon, uint8_t version, uint8_t CRC, uint8_t RXS_Max, 
        uint8_t TXS_Max, uint8_t TXA_Max, uint8_t CRCH);
void TLPacket_PINGSet(PING_t *ping, uint8_t C, uint8_t N, uint8_t LLID, uint16_t Number, uint8_t CRCH);


/*
 * Create and free TLPackets
 */
TLPacket_t *TLPacket_Create(TLPacketHeader_t *header, int type, uint8_t *payload, int payload_size);
void TLPacket_Free(TLPacket_t *packet);


//Checks for an ERROR packet and returns it's error code
int TLPacket_checkError(uint8_t *PKTbuf);

//Returns packet type
uint8_t TLPacket_getType(uint8_t *PKTbuf);

//Checks if TLPacket is of a valid type and it's CRCH
bool TLPacket_checkBadPacket(uint8_t *PKTbuf);

#endif
