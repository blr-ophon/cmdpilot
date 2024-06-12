#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "mcp.h"


int Serialize_Command(Command_t *command, uint8_t *buf);

//TODO: define packet struct on mpc.h
//int Serialize_Packet(Packet_t *packet, uint8_t *buf);

#endif
