#include <string.h>
#include "serialize.h"

#define MASK1 0x01
#define MASK3 0x07
#define MASK4 0x0f
#define MASK6 0x3f
#define MASK7 0x7f
#define MASK13 0x1fff

/* Local Functions */
static uint32_t Serialize_Header(PacketHeader_t *header, int Packet_type);



/*
 * Serialize packet struct into byte stream
 */
int Serialize_Packet(Packet_t *packet, uint8_t *buf, int buf_size){
    int rv = 0;
    if(buf_size < 6 + packet->Payload_size){
        rv = -1;
        goto out;
    }
    
    uint32_t Header = Serialize_Header(&packet->header, packet->type);

    /* Header + payload + CRC */
    int i = 0;
    memcpy(&buf[i], &Header, 4);
    i += 4;
    memcpy(&buf[i], packet->Payload, packet->Payload_size);
    i += packet->Payload_size;
    memcpy(&buf[i], &packet->CRC, 2);
     
out:
    return rv;
}



/*
 * Serialize Command struct into byte stream
 */
int Serialize_Command(Command_t *command, uint8_t *buf, int buf_size){
    int rv = 0;
    if(buf_size < 2 + command->payload_size){   
        rv = -1;
        goto out;
    }

    int i = 0;
    uint16_t header = (command->motor_id) | (command->command_id << 3);
    memcpy(&buf[i], &header, 2);
    i+= 2;
    memcpy(&buf[i], command->payload, command->payload_size);

out:
    return rv;
}


static uint32_t Serialize_Header(PacketHeader_t *header, int Packet_type){
    uint32_t rv = 0;
    switch(Packet_type){
        case PKTTYPE_BEACON:    
            rv |= (header->Beacon.type & MASK4)     << 0; 
            rv |= (header->Beacon.version & MASK3)  << 4;
            rv |= (header->Beacon.CRC & MASK1)      << 7; 
            rv |= (header->Beacon.RXS_Max & MASK6)  << 8; 
            rv |= (header->Beacon.TXS_Max & MASK7)  <<14; 
            rv |= (header->Beacon.TXA_Max & MASK7)  <<21; 
            rv |= (header->Beacon.CRCH & MASK4)     <<28; 
            break;
        case PKTTYPE_PING:
            rv |= (header->Ping.type & MASK4)       << 0;
            rv |= (header->Ping.C & MASK1)          << 4;
            rv |= (header->Ping.C & MASK1)          << 5;
            rv |= (header->Ping.N & MASK1)          << 6;
            rv |= (header->Ping.N & MASK1)          << 7;
            rv |= (header->Ping.LLID & MASK4)       << 8;
            rv |= (header->Ping.Number)             <<12;
            rv |= (header->Ping.CRCH & MASK4)       <<28; 
            break;
        case PKTTYPE_ERROR:
            rv |= (header->Error.type & MASK4)      << 0;
            rv |= (header->Error.ErrorCode)         << 8;
            rv |= (header->Error.ErrorCode)         <<16;
            rv |= (header->Error.CRCH & MASK4)      <<28; 
            break;
        case PKTTYPE_REQUEST_ASYNC:
            rv |= (header->Async.type & MASK4)      << 0;
            rv |= (header->Async.payloadLength & MASK13) << 4;
            rv |= (header->Async.CRCH & MASK4)      <<28; 
            break;
        case PKTTYPE_RESPONSE:
            rv |= (header->Response.type & MASK4)   << 0;
            rv |= (header->Response.payloadLength & MASK13) << 4;
            rv |= (header->Response.CRCH & MASK4)   <<28; 
            break;
        default:
            break;
    }
    return rv;
}
