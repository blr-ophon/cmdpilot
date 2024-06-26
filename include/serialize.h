#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "packet.h"
#include "mcp.h"


int Serialize_Packet(Packet_t *packet, uint8_t *buf, int buf_size);
int Serialize_Command(Command_t *command, uint8_t *buf, int buf_size);

#endif
