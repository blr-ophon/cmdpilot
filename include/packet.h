#ifndef PACKET_H
#define PACKET_H

/*
 * Functions for packet encoding, decoding and CRC check
 */

#include <stdint.h>
#include <stdint.h>

#define BADCRC  1

typedef enum{
    PKTTYPE_BEACON               =   5,
    PKTTYPE_PING                 =   6,
    PKTTYPE_ERROR                =   15,
    PKTTYPE_REQUEST_ASYNC        =   9,
    PKTTYPE_RESPONSE             =   10,
}Packet_type;


////////////////////////////////////////////////////////////////////////////////
///32-bit fields of packet headers
////////////////////////////////////////////////////////////////////////////////

typedef struct{
    uint8_t type:4;
    uint8_t version:3;
    uint8_t CRC:1;
    uint8_t RXS_Max:6;
    uint8_t TXS_Max:7;
    uint8_t TXA_Max:7;
    uint8_t CRCH:4;
}__attribute__((packed)) BEACON_t;

typedef struct{
    uint8_t type:4;
    uint8_t C:2;
    uint8_t N:2;
    uint8_t LLID:4;
    uint16_t Number;
    uint8_t CRCH:4;
}__attribute__((packed)) PING_t;

typedef struct{
    uint8_t type:4;
    uint8_t reserved1:4;
    uint8_t ErrorCode1;
    uint8_t ErrorCode2;
    uint8_t reserved2:4;
    uint8_t CRCH:4;
}__attribute__((packed)) ERROR_t;

typedef struct{
    uint8_t type:4;
    uint16_t payloadLength:13;
    uint16_t reserved:11;
    uint8_t CRCH:4;
}__attribute__((packed)) ASYNC_t;

typedef struct{
    uint8_t type:4;
    uint16_t payloadLength:13;
    uint16_t reserved:11;
    uint8_t CRCH:4;
}__attribute__((packed)) RESPONSE_t;

////////////////////////////////////////////////////////////////////////////////

typedef union{
    BEACON_t Beacon;
    PING_t Ping;
    ERROR_t Error;
    ASYNC_t Async;
    RESPONSE_t Response;
}PacketHeader_t;


typedef struct{
    PacketHeader_t header;
    uint8_t *Payload;
    uint16_t CRC;               //Optional
    uint8_t Payload_size;
}Packet_t;

////////////////////////////////////////////////////////////////////////////////
/// Functions
////////////////////////////////////////////////////////////////////////////////

int Packet_CRCCheck(Packet_t *packet);
int Packet_CRCHCheck(PacketHeader_t *header);
int Packet_setBeacon(PacketHeader_t *header, uint8_t type, uint8_t version, 
        uint8_t CRC, uint8_t CRCH, uint8_t RXS_Max, uint8_t TXA_Max, uint8_t TXS_Max);


#endif
