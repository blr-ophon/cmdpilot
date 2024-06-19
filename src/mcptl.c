#include "mcptl.h"

//TODO: Every packet has a response, except asynchronous ones.
//If CRC check failed, the sent one must be resent
//There is no send and receive on the API, only send-receive pair,
//all commands that involve receive require a previous send on the
//controller side

#define MIN(a,b) ( ((a) < (b)) ? (a) : (b) )

static bool MCPTL_CRCCheck(uint8_t *packet, int size);
static int MCPTL_sendPacket(MCPTL_handle *pHandle, uint8_t *buf, int n);
static int MCPTL_recvPacket(MCPTL_handle *pHandle, uint8_t *buf, int *n);
static int MCPTL_sendSYNC(MCPTL_handle *pHandle, uint8_t flags);



/*
 * Creates an MCPTL socket that contains two independent channels
 */
MCPTL_handle *MCPTL_getHandle(void){
    return 0;
}

/*
 * Sends Beacon and awaits for response. If response is equal, go to
 * connecting state, otherwise go to configuring state
 */
int MCPTL_stateIDLE(MCPTL_handle *pHandle, const BEACON_t *const LocalBeacon){
    int rv = 0;     //timeout, error

    //Create packet 
    Packet_t packet;
    packet.header.Beacon = *LocalBeacon;
    packet.Payload = 0;
    packet.CRC = 0;

    //Serialize packet and move to sync channel
    uint8_t buf[256];
    Serialize_Packet(&packet, buf, 256);
    memcpy(pHandle->rxBuf, buf, 256);

    //Send packet and check for error response
    bool repeat = false;
    do{
        MCPTL_sendSYNC(pHandle, RETURN_ERROR);
        int type = Packet_getType(pHandle->txBuf);
        switch(type){
            case PKTTYPE_BEACON:
                pHandle->state = STATE_CONFIGURING;
                repeat = false;
                break;
            case PKTTYPE_ERROR:
                pHandle->state = STATE_IDLE;
                rv = Packet_checkError(pHandle->txBuf);
                repeat = false;
                break;
            default:
                //Keep sending
                break;
        }
    }while(repeat);

    return rv;
}

/*
 * Exchanges Beacon with performer until both agree on parameters
 */
int MCPTL_stateCONFIG(MCPTL_handle *pHandle, const BEACON_t * const LocalBeacon){
    int rv = 0;     //timeout, error

    //Create packet 
    Packet_t packet;
    packet.header.Beacon = *LocalBeacon;
    packet.Payload = 0;
    packet.CRC = 0;

    //Serialize packet and move to sync channel
    uint8_t buf[256];
    Serialize_Packet(&packet, buf, 256);
    memcpy(pHandle->rxBuf, buf, 256);

    BEACON_t PerformerBeacon;
    bool repeat = true;
    do{
        //Send packet and receive response
        MCPTL_sendSYNC(pHandle, RETURN_ERROR);

        int type = Packet_getType(pHandle->txBuf);
        switch(type){
            case PKTTYPE_BEACON:
                //Keep modifying and changing beacons until the response is the same
                if(!memcmp(pHandle->rxBuf, pHandle->txBuf, sizeof(PacketHeader_t))){
                    pHandle->state = STATE_CONNECTING;
                    repeat = false;
                }else{
                    Beacon_decode(&PerformerBeacon, pHandle->rxBuf);
                    packet.header.Beacon = PerformerBeacon;
                }
                break;
            case PKTTYPE_ERROR:
                pHandle->state = STATE_IDLE;
                rv = Packet_checkError(pHandle->txBuf);
                repeat = false;
                break;
            default:
                //Keep sending beacons
                break;
        }
    }while(repeat);


    return rv;
}

/*
 * Sends PING and awaits for response. If there is a PING response,
 * the devices go to connected state
 */
int MCPTL_stateCONNECT(MCPTL_handle *pHandle){
    int rv = 0;     //timeout, error
    //Create packet 
    Packet_t packet;
    Ping_set(&packet.header.Ping,0,0,0,0,0);
    packet.Payload = 0;
    packet.CRC = 0;

    //Serialize packet and move to sync channel
    uint8_t buf[256];
    Serialize_Packet(&packet, buf, 256);
    memcpy(pHandle->rxBuf, buf, 256);

    //Send packet and check for error response
    bool repeat = false;
    do{
        MCPTL_sendSYNC(pHandle, RETURN_ERROR);
        int type = Packet_getType(pHandle->txBuf);
        switch(type){
            case PKTTYPE_PING:
                repeat = false;
                pHandle->state = STATE_CONNECTED;
                break;
            case PKTTYPE_ERROR:
                repeat = false;
                pHandle->state = STATE_IDLE;
                rv = Packet_checkError(pHandle->txBuf);
                break;
            default:
                //Keep sending
                break;
        }
    }while(repeat);

    return rv;
}




/*
 * Send whatever is on Synchronous channel, checks
 * CRC of response
 * Supposes packet is already serialized in pHandle buffer.
 * Maybe its better if packet is sent as argument (TODO)
 */
static int MCPTL_sendSYNC(MCPTL_handle *pHandle, uint8_t flags){
    //TODO: number of bytes to send and received inside handle
    int rv = 0;
    if(pHandle->state != STATE_CONNECTED){ 
        rv = -1;
        goto out;
    }

    int recv_bytes = 0;
    bool crc_ok = false;
    int error_code = 0;

    //Main loop
    while(recv_bytes == 0 && !crc_ok && error_code){
        MCPTL_sendPacket(pHandle, pHandle->rxBuf, 256);
        //TODO:wait T ms
        
        //read response
        MCPTL_recvPacket(pHandle, pHandle->txBuf, &recv_bytes);
        error_code = Packet_checkError(pHandle->txBuf);
        crc_ok = MCPTL_CRCCheck(pHandle->txBuf, recv_bytes);

        if(recv_bytes && crc_ok && (flags & RETURN_ERROR)){
            break;
        }
    }
out:
    return rv;
}

/*
 * Same as SYNC, but receives multiple responses and cant do a
 * CRC check
 * TODO: SYNC/ASYNC as flag instead of another function
 */
int MCPTL_sendASYNC(MCPTL_handle *pHandle, int *n){
    return 0;
}



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
