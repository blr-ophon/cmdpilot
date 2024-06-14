#include "mcptl.h"


/*
 * local function to send any type of packet without verifying 
 * that its connected
 */
static int MCPTL_sendData(MCPTL_handle *pHandle, uint8_t *buf, int n){
    //Call low level functions for UART communication
}
static int MCPTL_recvData(MCPTL_handle *pHandle, uint8_t *buf, int *n){
    //Call low level functions for UART communication
}



/*
 * Creates an MCPTL socket that contains two independent channels
 */
MCPTL_handle *MCPTL_getHandle(void){
    return 0;
}



int MCPTL_connect(MCPTL_handle *pHandle){
    while(pHandle->state != STATE_CONNECTED){
        switch(pHandle->state){
            case STATE_IDLE:
                //Send beacon
                //read response
                //if response is the same, state = connecting
                //else, change beacon and resend,state = configuring
                break;
            case STATE_CONFIGURING:
                //Send beacon
                //read response, check crc
                //if response is the same, state = connecting
                //else, change beacon and resend
                break;
            case STATE_CONNECTING:
                //Send ping 
                //read response, check crc
                //if response is positive, state = connected
                //else, ?
                break;
            default:
                break;
                //TODO: Bad packet
        }
    }
    return 0;
}


int MCPTL_send(MCPTL_handle *pHandle, uint8_t *buf, int *n){
    if(pHandle->state == STATE_CONNECTED){ 
        //send header (first 4 bytes). 
        MCPTL_sendData(pHandle, &buf[0], 4);
        //intra-packet pause
        //send payload
        MCPTL_sendData(pHandle, &buf[4], (*n) - 4);
    }else{
        //TODO: return error code for ui to print on stderr
    }
    return 0;
}

int MCPTL_recv(MCPTL_handle *pHandle, uint8_t *buf, int *n){
    //Receive Header
    //Receive Payload
    //Receive CRC
    
    //If bad CRC, send a packet notifying
    //bad CRC and wait for new packet
    return 0;
}

