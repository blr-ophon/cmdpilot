#include "mcptl.h"

//TODO: Every packet has a response, except asynchronous ones.
//If CRC check failed, the sent one must be resent
//There is no send and receive on the API, only send-receive pair,
//all commands that involve receive require a previous send on the
//controller side

#define MIN(a,b) ( ((a) < (b)) ? (a) : (b) )

/*
 * local function to send any type of packet without verifying 
 * that its connected
 */
static int MCPTL_sendPacket(MCPTL_handle *pHandle, uint8_t *buf, int n){
    //TODO: checks on handle, buf and n
    uint8_t *offset = pHandle->txBuf;
    int payload_size = n - 6;

    //send Header
    int rv = UART_Send(pHandle->fd, offset, 4);
    offset += 4;

    //TODO:intra-packet pause
    //send payload
    UART_Send(pHandle->fd, offset, payload_size);
    offset += payload_size;

    //send CRC
    UART_Send(pHandle->fd, offset, 2);
    return rv;
}

static int MCPTL_recvPacket(MCPTL_handle *pHandle, uint8_t *buf, int *n){
    //TODO: checks on handle, buf and n
    int rv = UART_Recv(pHandle->fd, buf, 256);
    if(rv < 0){
        //Error reading from UART
    }else{
        *n = rv;
    }
    return rv;
}

static bool MCPTL_CRCCheck(uint8_t *packet, int size){
#ifdef CRC_CHECK
    uint16_t crc = packet[size-2];
    if(0){
        return -BADCRC;
    }
#endif
    return true;
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
                //TODO: Use sendSYNC. Beacons are synchronous
                uint8_t buf[256];
                //Create packet 
                Packet_t packet;
                Packet_setBeacon(&packet,0,0,0,0,0,0,0);
                packet.Payload = 0;
                packet.CRC = 0;

                //Serialize packet and send until a response is received
                Serialize_Packet(&packet, buf, 256);
                int send_size = 256;
                MCPTL_sendSYNC(pHandle, &send_size);

                int a = memcmp(pHandle->rxBuf, pHandle->txBuf, sizeof(PacketHeader_t));
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


/*
 * Send whatever is on Synchronous channel, checks
 * CRC of response
 */
int MCPTL_sendSYNC(MCPTL_handle *pHandle, int *n){
    //TODO: number of bytes to send and received inside handle
    int rv;
    if(pHandle->state != STATE_CONNECTED){ 
        rv = -1;
        goto out;
    }

    int recv_bytes = 0;
    bool crc_ok = false;
    //send header (first 4 bytes). 
    uint8_t *offset = pHandle->txBuf;
    int payload_size = *n - 6;
    if(payload_size < 0){
        rv = -2;
        goto out;
    }

    //Main loop
    while(recv_bytes == 0 && !crc_ok){
        MCPTL_sendPacket(pHandle, pHandle->rxBuf, 256);
        //TODO:wait T ms
        
        //read response
        MCPTL_recvPacket(pHandle, pHandle->txBuf, &recv_bytes);
        crc_ok = MCPTL_CRCCheck(pHandle->txBuf, recv_bytes);
    }

out:
    return rv;
}

/*
 * Same as SYNC, but receives multiple responses and cant do a
 * CRC check
 */
int MCPTL_sendASYNC(MCPTL_handle *pHandle, int *n){
    return 0;
}
