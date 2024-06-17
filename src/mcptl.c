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
                {
                uint8_t buf[256];
                //Create packet 
                BEACON_t Beacon;
                Packet_setBeacon(0,0,0,0,0,0,0,0);
                Packet_t Packet;
                Packet.header.Beacon = Beacon;
                Packet.Payload = 0;
                Packet.CRC = 0;
                //Serialize packet and send
                Serialize_Packet(&Packet, buf, 256);
                //TODO: Use MPTL_sendData for automatic buffer/channel handling
                UART_Send(pHandle->fd, pHandle->txBuf, 256);

                //read response
                //TODO:wait?
                UART_Recv(pHandle->fd, pHandle->rxBuf, 256);
                
                //if response is the same, state = connecting
                //else, change beacon, state = configuring
                //TODO: check if they have same size first, then use their size
                int a = memcmp(pHandle->rxBuf, pHandle->txBuf, 256);
                if(a == 0){
                    pHandle->state = STATE_CONNECTING;
                }else{
                    //TODO: change beacon;
                    pHandle->state = STATE_CONFIGURING;
                }
                break;
                }
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
                //TODO: Bad packet / Handle
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

