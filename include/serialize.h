#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "mcp.h"
#include "packet.h"


int Serialize_Packet(Packet_t *packet, uint8_t *buf, int buf_size);

//TODO: define packet struct on mpc.h
//int Serialize_Packet(Packet_t *packet, uint8_t *buf);

#endif
