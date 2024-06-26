#include "mcp.h"
#include "serialize.h"

/*
 * Creates an MCPTL socket with two independent channels
 * and user defined parameters
 */
MCPTL_handle *MCP_createHandle(char *port, long baudRate){
    MCPTL_handle *pHandle = malloc(sizeof(MCPTL_handle));
    pHandle->fd = UART_getfd(port, baudRate);
    pHandle->state = STATE_IDLE;
    pHandle->RXS_Max = RXS_MAX;
    pHandle->TXS_Max = TXS_MAX;
    pHandle->TXA_Max = TXA_MAX;

    return pHandle;
}

void MCP_freeHandle(MCPTL_handle *pHandle){
    free(pHandle);
}


/*
 * TODO
 */
int MCP_keepAlive(MCPTL_handle *pHandle, bool condition){
    int rv = 0;

    bool alive = false;
    if(condition){
        //send PING
        //receive and check response
        alive = true;
    }

    int timeout = 5;    //seconds
    time_t startTime = time(NULL);
    time_t curTime;
    while(!alive){
        rv = MCP_connect(pHandle);
        curTime = time(NULL);
        if(curTime-startTime >= timeout){
            perror("Keep Alive Timeout");
            rv = -EKEEPALIVE_TIMEOUT;
            break;
        }
    }
    
    return rv; 
}


int MCP_connect(MCPTL_handle *pHandle){
    int rv = 0;
    //restart connection from the beginning
    BEACON_t LocalBeacon;
    Beacon_set(&LocalBeacon, MCPVERSION, 0, RXS_MAX, TXS_MAX, TXS_MAX, 0);

    while(pHandle->state != STATE_CONNECTED){
        switch(pHandle->state){
            case STATE_IDLE:
                /*
                 * Send BEACON until receiving a BEACON.
                 */
                MCPTL_stateIDLE(pHandle, &LocalBeacon);
                break;
            case STATE_CONFIGURING:
                /*
                 * Send modified BEACONs until the performer agrees.
                 */
                MCPTL_stateCONFIG(pHandle, &LocalBeacon);
                break;
            case STATE_CONNECTING:
                /*
                 * Send PING until a PING response is received
                 */
                MCPTL_stateCONNECT(pHandle);
                break;
            default:
                perror("Bad handle");
                rv = -1;
                goto out;
        }
    }

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
    UART_Send(pHandle->fd, pHandle->SYNCtxBuf, 256);

out:
    return rv;
}


/*
 * Read reponse from serial port
 */
void MCP_recvResponse(MCPTL_handle *pHandle, Response_t *response){
    //Receive Response
    int recv_bytes = UART_Recv(pHandle->fd, pHandle->SYNCrxBuf, 256);
    response->status_code = pHandle->SYNCrxBuf[recv_bytes-1];
    if(response->status_code == RP_CMD_OK){
        //read payload    
        memcpy(response->payload, pHandle->SYNCrxBuf, recv_bytes-1);
    }
}

/* TODO */
int MCP_Datalog(MCPTL_handle *pHandle, uint8_t *payload, int pl_len){
    //Create command packet with specified command id
    //sendASYNC
    //decode response and return it
    return 0;
}



