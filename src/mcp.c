#include "mcp.h"
#include "serialize.h"

/*
 * TODO
 * Creates an MCPTL socket with two independent channels
 * and user defined parameters
 */
MCPTL_handle *MCP_getHandle(void){
    //Configure UART, get file descriptor
    //Get parameters from a config.h file
    return 0;
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
                break;
                //TODO: Bad packet / Handle
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
        //TODO: error handling
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
int MCP_recvResponse(MCPTL_handle *pHandle){
    //Receive Response
    int recv_bytes = UART_Recv(pHandle->fd, pHandle->SYNCrxBuf, 256);
    uint8_t status_code = pHandle->SYNCrxBuf[recv_bytes-1];
    if(status_code == RP_CMD_OK){
        
    }
    int rv = 0;

    return rv;
}

/* TODO */
int MCP_Datalog(MCPTL_handle *pHandle, uint8_t *payload, int pl_len){
    //Create command packet with specified command id
    //sendSYNC
    //decode response and return it
    return 0;
}



