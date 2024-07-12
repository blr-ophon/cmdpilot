#include "mcp.h"
#include "serialize.h"


/*
 * Creates an MCPTL Handle containing the communication channels
 * To be used in the transport layer
 */
MCPTL_handle *MCP_createHandle(char *port, long baudRate){
    int fd = UART_getfd(port, baudRate);
    if(fd < 0){
        return NULL;
    }

    MCPTL_handle *pHandle = calloc(1, sizeof(MCPTL_handle));
    pHandle->fd = fd;
    pHandle->state = STATE_IDLE;
    pHandle->RXS_Max = RXS_MAX;
    pHandle->TXS_Max = TXS_MAX;
    pHandle->TXA_Max = TXA_MAX;

    return pHandle;
}

void MCP_freeHandle(MCPTL_handle *pHandle){
    free(pHandle);
}




int MCP_connect(MCPTL_handle *pHandle){
    int rv = 0;
    //restart connection from the beginning
    BEACON_t LocalBeacon;
    TLPacket_BEACONSet(&LocalBeacon, MCPVERSION, 0, RXS_MAX, TXS_MAX, TXA_MAX, 0);

    while(pHandle->state != STATE_CONNECTED){
        switch(pHandle->state){
            case STATE_IDLE:
                /*
                 * Send BEACON until receiving a BEACON.
                 */
                printf("State: IDLE\n");
                if(MCPTL_stateIDLE(pHandle, &LocalBeacon) < 0){
                    printf("(MCP_connect) Unable to start configuring\n");
                    rv = -1;
                    goto out;
                }
                break;
            case STATE_CONFIGURING:
                printf("State: CONFIGURING\n");
                /*
                 * Send modified BEACONs until the performer agrees.
                 */
                MCPTL_stateCONFIG(pHandle, &LocalBeacon);
                break;
            case STATE_CONNECTING:
                printf("State: CONNECTING\n");
                /*
                 * Send PING until a PING response is received
                 */
                MCPTL_stateCONNECT(pHandle);
                break;
            default:
                printf("MCP_connect: Bad Handle\n");
                rv = -1;
                goto out;
        }
    }

    printf("State: CONNECTED\n");


out:
    return rv;
}

/* 
 * Send command via UART after connection has been stablished
 */
int MCP_sendCommand(MCPTL_handle *pHandle, uint8_t motor_id, uint8_t command_id, uint8_t *payload, int pl_len){
    int rv = 0;
    if(pHandle->state != STATE_CONNECTED){
        rv = -1;
        goto out;
    }
    Command_t command;
    command.motor_id = motor_id;
    command.command_id = command_id;
    command.payload = payload;
    command.payload_size = pl_len;

    uint8_t buf[256];
    Serialize_Command(&command, buf, 256);
    /*
     * TODO
     * Encapsulate command into REQUEST packet
     * Call sendSYNC
     * Decapsulate RESPONSE packet
     */

out:
    return rv;
}


/*
 * Read reponse from serial port
 */
void MCP_recvResponse(MCPTL_handle *pHandle, Response_t *response){
    //Receive Response
    int recv_bytes = UART_Recv(pHandle->fd, pHandle->SYNCrxbuf, 256);
    response->status_code = pHandle->SYNCrxbuf[recv_bytes-1];
    if(response->status_code == RP_CMD_OK){
        //read payload    
        memcpy(response->payload, pHandle->SYNCrxbuf, recv_bytes-1);
    }
    //TODO: encapsulate into response packet
}

/*
 * TODO
 */
int MCP_keepAlive(MCPTL_handle *pHandle, bool condition){
    int rv = 0;

    bool connected = false;
    if(condition){
        //send PING
        //receive and check response
        connected = true;
    }
    if(!connected){ 
        rv |= MCP_connect(pHandle);
    }
    
    return rv; 
}

/* TODO */
int MCP_Datalog(MCPTL_handle *pHandle, uint8_t *payload, int pl_len){
    //Create command packet with specified command id
    //sendASYNC
    //decode response and return it
    return 0;
}



