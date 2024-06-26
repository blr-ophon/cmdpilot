#include "mcptl.h"
#include "serialize.h"
#include "decode.h"

//If CRC check failed, the sent one must be resent
//There is no send and receive on the API, only send-receive pair,
//all commands that involve receive require a previous send on the
//controller side

#define MIN(a,b) ( ((a) < (b)) ? (a) : (b) )

/* Local Functions */
static bool MCPTL_CRCCheck(uint8_t *packet, int size);
static int MCPTL_sendPacket(MCPTL_handle *pHandle, uint8_t channel, int n);
static int MCPTL_recvPacket(MCPTL_handle *pHandle, uint8_t channel, int *n);
static int MCPTL_sendCTRL(MCPTL_handle *pHandle);
static int MCPTL_sendASYNC(MCPTL_handle *pHandle, uint8_t flags);
static int MCPTL_CONFIG_UpdateBeacon(BEACON_t *LocalBeacon, BEACON_t *PerformerBeacon);



/*
 * Sends Beacon and awaits for response. If response is equal, go to
 * connecting state, otherwise go to configuring state
 */
int MCPTL_stateIDLE(MCPTL_handle *pHandle, const BEACON_t *const LocalBeacon){
    int rv = 0;     //timeout, error

    //Create packet 
    Packet_t packet;
    packet.header.Beacon = *LocalBeacon;
    packet.Payload = NULL;
    packet.Payload_size = 0;

    //Serialize packet and move to sync channel
    uint8_t buf[BEACON_SIZE];
    Serialize_Packet(&packet, buf, BEACON_SIZE);
    memcpy(pHandle->CTRLrxbuf, buf, BEACON_SIZE);

    //Send packet and check for error response
    bool repeat = false;
    do{
        MCPTL_sendCTRL(pHandle);
        int type = Packet_getType(pHandle->CTRLrxbuf);
        switch(type){
            case PKTTYPE_BEACON:
                pHandle->state = STATE_CONFIGURING;
                repeat = false;
                break;
            case PKTTYPE_ERROR:
                pHandle->state = STATE_IDLE;
                rv = Packet_checkError(pHandle->CTRLrxbuf);
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
int MCPTL_stateCONFIG(MCPTL_handle *pHandle, BEACON_t * LocalBeacon){
    int rv = 0;     //timeout, error

    //Create packet 
    Packet_t packet;
    packet.header.Beacon = *LocalBeacon;
    packet.Payload = 0;
    packet.CRC = 0;

    //Serialize packet and move to control channel
    uint8_t buf[256];
    Serialize_Packet(&packet, buf, 256);
    memcpy(pHandle->CTRLtxbuf, buf, 256);

    BEACON_t PerformerBeacon;
    bool repeat = true;
    do{
        //Send packet and receive response
        MCPTL_sendCTRL(pHandle);
        uint8_t *ResponseBuf = pHandle->CTRLrxbuf;

        int type = Packet_getType(ResponseBuf);
        switch(type){
            case PKTTYPE_BEACON:
                //Keep modifying and changing beacons until the response is the same
                if(!memcmp(pHandle->CTRLrxbuf, ResponseBuf, sizeof(PacketHeader_t))){
                    pHandle->state = STATE_CONNECTING;
                    repeat = false;
                }else{
                    Beacon_decode(&PerformerBeacon, ResponseBuf);
                    MCPTL_CONFIG_UpdateBeacon(LocalBeacon, &PerformerBeacon);
                }
                break;
            case PKTTYPE_ERROR:
                pHandle->state = STATE_IDLE;
                rv = Packet_checkError(ResponseBuf);
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
    memcpy(pHandle->CTRLtxbuf, buf, 256);

    //Send packet and check for error response
    bool repeat = false;
    do{
        MCPTL_sendCTRL(pHandle);
        uint8_t *ResponseBuf = pHandle->CTRLrxbuf;
        int type = Packet_getType(ResponseBuf);
        switch(type){
            case PKTTYPE_PING:
                repeat = false;
                pHandle->state = STATE_CONNECTED;
                break;
            case PKTTYPE_ERROR:
                repeat = false;
                pHandle->state = STATE_IDLE;
                rv = Packet_checkError(ResponseBuf);
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
 */
static int MCPTL_sendCTRL(MCPTL_handle *pHandle){
    //TODO: number of bytes to send and received inside handle
    int rv = 0;
    if(pHandle->state != STATE_CONNECTED){ 
        rv = -1;
        goto out;
    }
    memset(pHandle->CTRLrxbuf, 0, 256);

    int recv_bytes = 0;
    bool crc_ok = false;
    int error_code = 0;

    //Main loop
    while(recv_bytes == 0 && !crc_ok && error_code){
        MCPTL_sendPacket(pHandle, CHANNEL_CTRL, 256);
        //TODO:wait T ms
        
        //read response
        MCPTL_recvPacket(pHandle, CHANNEL_CTRL, &recv_bytes);
        error_code = Packet_checkError(pHandle->SYNCrxBuf);
        crc_ok = MCPTL_CRCCheck(pHandle->SYNCrxBuf, recv_bytes);
    }
out:
    return rv;
}

/*
 * Same as SYNC, but receives multiple responses and cant do a
 * CRC check
 * TODO: SYNC/ASYNC as flag instead of another function
 */
static int MCPTL_sendASYNC(MCPTL_handle *pHandle, uint8_t flags){
    return 0;
}



/*
 * local function to send any type of packet without verifying 
 * that its connected
 */
static int MCPTL_sendPacket(MCPTL_handle *pHandle, uint8_t channel, int n){
    int rv = 0;
    //TODO: checks on handle, buf and n
    uint8_t *txBuf;
    switch(channel){
        case CHANNEL_CTRL:
            txBuf = pHandle->CTRLtxbuf;
            break;
        case CHANNEL_SYNC:
            txBuf = pHandle->SYNCtxBuf;
            break;
        case CHANNEL_ASYNC:
            //TODO
            goto out;
            break;
        default:
            perror("Invalid channel");
            rv = -1;
            goto out;
    }

    uint8_t *offset = txBuf;
    int payload_size = n - 6;

    //send Header
    rv = UART_Send(pHandle->fd, offset, 4);
    offset += 4;

    //TODO:intra-packet pause
    //send payload
    UART_Send(pHandle->fd, offset, payload_size);
    offset += payload_size;

    //send CRC
    UART_Send(pHandle->fd, offset, 2);

    Decode_Packet(txBuf, 0);
out:
    return rv;
}


static int MCPTL_recvPacket(MCPTL_handle *pHandle, uint8_t channel, int *n){
    int rv = 0;

    uint8_t *rxBuf;
        switch(channel){
        case CHANNEL_CTRL:
            rxBuf = pHandle->CTRLrxbuf;
            break;
        case CHANNEL_SYNC:
            rxBuf = pHandle->SYNCrxBuf;
            break;
        case CHANNEL_ASYNC:
            //TODO
            goto out;
            break;
        default:
            perror("Invalid channel");
            rv = -1;
            goto out;
    }

    //TODO: checks on handle, buf and n
    rv = UART_Recv(pHandle->fd, rxBuf, 256);
    if(rv < 0){
        //Error reading from UART
    }else{
        *n = rv;
    }

    Decode_Packet(rxBuf, 1);

out:
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

static int MCPTL_CONFIG_UpdateBeacon(BEACON_t *LocalBeacon, BEACON_t *PerformerBeacon){
    LocalBeacon->RXS_Max = MIN(LocalBeacon->RXS_Max, PerformerBeacon->RXS_Max);
    LocalBeacon->TXS_Max = MIN(LocalBeacon->TXS_Max, PerformerBeacon->TXS_Max);
    LocalBeacon->TXA_Max = MIN(LocalBeacon->TXA_Max, PerformerBeacon->TXA_Max);
}
