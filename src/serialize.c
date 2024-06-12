#include "serialize.h"




/*
 * Serialize Commands and unserialize response
 */
int Serialize_Command(Command_t *command, uint8_t *buf){
    int rv = 0;

    uint8_t command_packet[256] = {0};
    uint8_t response_packet[256] = {0};

    //Serialize command
    memcpy(&command_packet[0], command, sizeof(uint16_t));
    memcpy(&command_packet[2], command->payload, command->payload_size);
     
    switch(command->command_id){
        case CID_GET_MCP_VERSION:
            break;
        case CID_SET_REGISTER:
            break;
        case CID_GET_REGISTER:
            break;
        case CID_START_MOTOR:
            break;
        case CID_STOP_RAMP:
            break;
        case CID_START_STOP:
            break;
        case CID_FAULT_ACK:
            break;
        default:
            rv = -EINVCOMMAND; 
            goto out;
    }

out:
    return rv;
}

