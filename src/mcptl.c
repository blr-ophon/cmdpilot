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
static int MCPTL_sendTLPacket(MCPTL_handle *pHandle, uint8_t channel, int n);
static int MCPTL_recvTLPacket(MCPTL_handle *pHandle, uint8_t channel, int *n);
static int MCPTL_sendCTRL(MCPTL_handle *pHandle);
static int MCPTL_sendASYNC(MCPTL_handle *pHandle, uint8_t flags);
static int MCPTL_CONFIG_UpdateBeacon(BEACON_t *LocalBeacon, BEACON_t *PerformerBeacon);
static int MCPTL_BufDump(MCPTL_handle *pHandle);

void test_sendBeacon(MCPTL_handle *pHandle){
    BEACON_t LocalBeacon;
    TLPacket_BEACONSet(&LocalBeacon, MCPVERSION, 0, RXS_MAX, TXS_MAX, TXA_MAX, 0);

    //Create packet 
    TLPacket_t packet;
    packet.type = PKTTYPE_BEACON;
    packet.header.Beacon = LocalBeacon;
    packet.Payload = NULL;
    packet.Payload_size = 0;
    packet.CRC = 0;

    //Serialize packet and move to sync channel
    uint8_t buf[BEACON_SIZE] = {0};
    Serialize_Packet(&packet, buf, BEACON_SIZE);
    memcpy(pHandle->CTRLtxbuf, buf, BEACON_SIZE);
    MCPTL_BufDump(pHandle);

    //Send packet and check for error response
    Decode_Packet(pHandle->CTRLtxbuf, 0);
    MCPTL_sendCTRL(pHandle);
    /*
    MCPTL_sendTLPacket(pHandle, CHANNEL_CTRL, BEACON_SIZE);

    //Receive Response packet
    int recv_bytes = 0;
    while(recv_bytes == 0){
        MCPTL_recvTLPacket(pHandle, CHANNEL_CTRL, &recv_bytes);
    }
    //Decode received packet
    Decode_TLPacket(pHandle->CTRLrxbuf, 1);
    */
}



/*
 * Sends Beacon and awaits for response. If response is equal, go to
 * connecting state, otherwise go to configuring state
 */
int MCPTL_stateIDLE(MCPTL_handle *pHandle, const BEACON_t *const LocalBeacon){
    int rv = 0;     //timeout, error

    //Create packet 
    TLPacket_t packet;
    packet.type = PKTTYPE_BEACON;
    packet.Payload = NULL;
    packet.Payload_size = 0;
    packet.CRC = 0;
    //TLPacket_set(&packet, &pktheader, PKTTYPE_BEACON, NULL, 0);
    packet.header.Beacon = *LocalBeacon;
    
    //Serialize packet and move to sync channel
    uint8_t buf[BEACON_SIZE] = {0};
    Serialize_Packet(&packet, buf, BEACON_SIZE);
    memcpy(pHandle->CTRLtxbuf, buf, BEACON_SIZE);

    //Send packet. If an error response is received, resend previous packet.
    //If a bad packet is received, restart connection procedure.
    bool resend = true;
    while(resend){
        //send packet
        Decode_Packet(pHandle->CTRLtxbuf, 0);
        MCPTL_BufDump(pHandle);
        MCPTL_sendCTRL(pHandle);

        //Check for bad responses 
        if(TLPacket_checkBadPacket(pHandle->CTRLrxbuf)){
            pHandle->state = STATE_IDLE;
            rv = -1;
            return rv;
        }
        //Check for ERROR responses 
        resend = TLPacket_checkError(pHandle->CTRLrxbuf)? true : false;
    }

    int type = TLPacket_getType(pHandle->CTRLrxbuf);
    switch(type){
        case PKTTYPE_BEACON:
            pHandle->state = STATE_CONFIGURING;
            break;
        default: //Unexpected packet received
            pHandle->state = STATE_IDLE;
            rv = -1;
            break;
    }


    MCPTL_BufDump(pHandle);
    printf("\n");
    return rv;
}


/*
 * Send whatever is on Synchronous channel, checks
 * CRC of response
 * Supposes packet is already serialized in pHandle buffer.
 * Returns 0 if no error occurred or a negative value otherwise 
 */
static int MCPTL_sendCTRL(MCPTL_handle *pHandle){
    int rv = 0;

    int packets_sent = 0;
    int recv_bytes = 0;

    bool resend = true;
    //Main loop
    long trySend_time = Timer_currentMillis();
    for(; resend; packets_sent++){
        rv |= MCPTL_sendTLPacket(pHandle, CHANNEL_CTRL, 6);
        
        //try to read a response for 10ms
        recv_bytes = 0;
        long waitResp_time = Timer_currentMillis();
        while(recv_bytes <= 0){
            rv |= MCPTL_recvTLPacket(pHandle, CHANNEL_CTRL, &recv_bytes);
            if(Timer_checkTimeout(waitResp_time, 10)){
                break;
            }
        }
        if(recv_bytes > 0){
            resend = false;
        }else{
            //Keep resending for 5 seconds
            if(Timer_checkTimeout(trySend_time, NORESP_TIMEOUT)){
                printf("(MCPTL_sendCTRL)packet sent %d times. %d bytes received\n", 
                        packets_sent,
                        recv_bytes);
                printf("(MCPTL_sendCTRL)Timeout: No response from performer\n");
                rv = -1;
                resend = false;
            }
        }
    }

    return rv;
}


/*
 * Send packet of size n on tx buf of specified channel
 * returns a negative value if an error occurred
 */
static int MCPTL_sendTLPacket(MCPTL_handle *pHandle, uint8_t channel, int n){
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
            printf("(MCPTL_sendTLPacket) Invalid channel\n");
            rv = -1;
            goto out;
    }

    uint8_t *offset = txBuf;
    int payload_size = n - 6;

    //send Header
    rv |= UART_Send(pHandle->fd, offset, 4);
    offset += 4;

    if(payload_size){
        //intra-packet pause (1ms)
        Timer_msPause(1);
        //send payload
        rv |= UART_Send(pHandle->fd, offset, payload_size);
        offset += payload_size;
    }

    //send CRC
    rv |= UART_Send(pHandle->fd, offset, 2);

out:
    return rv;
}


static int MCPTL_recvTLPacket(MCPTL_handle *pHandle, uint8_t channel, int *n){
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



static int MCPTL_BufDump(MCPTL_handle *pHandle){
    printf("CHANNEL BUFFER:\n");
    printf("TX:\n");
    for(int i = 0; i < pHandle->TXS_Max; i++){
        printf("%x ",pHandle->CTRLtxbuf[i]);
    }
    printf("\nRX:\n");
    for(int i = 0; i < pHandle->RXS_Max; i++){
        printf("%x ",pHandle->CTRLrxbuf[i]);
    }
    printf("\n\n");
}









/*
 * Exchanges Beacon with performer until both agree on parameters
 */
int MCPTL_stateCONFIG(MCPTL_handle *pHandle, BEACON_t * LocalBeacon){
    int rv = 0;     //timeout, error

    //Create packet 
    TLPacket_t packet;
    packet.header.Beacon = *LocalBeacon;
    packet.Payload = 0;
    packet.CRC = 0;

    //Serialize packet and move to control channel
    uint8_t buf[256];
    Serialize_Packet(&packet, buf, 256);
    memcpy(pHandle->CTRLtxbuf, buf, 256);

    BEACON_t PerformerBeacon;
    long start_time = Timer_currentMillis();
    bool repeat = true;
    do{
        //Send packet and receive response
        MCPTL_sendCTRL(pHandle);
        uint8_t *ResponseBuf = pHandle->CTRLrxbuf;

        int type = TLPacket_getType(ResponseBuf);
        switch(type){
            case PKTTYPE_BEACON:
                //Keep modifying and changing beacons until the response is the same
                if(!memcmp(pHandle->CTRLrxbuf, ResponseBuf, sizeof(TLPacketHeader_t))){
                    pHandle->state = STATE_CONNECTING;
                    repeat = false;
                }else{
                    //TLPacket_BEACONDecode(&PerformerBeacon, ResponseBuf);
                    MCPTL_CONFIG_UpdateBeacon(LocalBeacon, &PerformerBeacon);
                }
                break;
            case PKTTYPE_ERROR:
                pHandle->state = STATE_IDLE;
                rv = TLPacket_checkError(ResponseBuf);
                repeat = false;
                break;
            default:
                //Keep sending beacons
                break;
        }
        if(Timer_checkTimeout(start_time, NORESP_TIMEOUT)){
            printf("Timeout: No Beacon Response from performer\n");
            repeat = false;
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
    TLPacket_t packet;
    TLPacket_PINGSet(&packet.header.Ping,0,0,0,0,0);
    packet.Payload = 0;
    packet.CRC = 0;

    //Serialize packet and move to sync channel
    uint8_t buf[256];
    Serialize_Packet(&packet, buf, 256);
    memcpy(pHandle->CTRLtxbuf, buf, 256);

    //Send packet and check for error response
    long start_time = Timer_currentMillis();
    bool repeat = true;
    do{
        MCPTL_sendCTRL(pHandle);
        uint8_t *ResponseBuf = pHandle->CTRLrxbuf;
        int type = TLPacket_getType(ResponseBuf);
        switch(type){
            case PKTTYPE_PING:
                repeat = false;
                pHandle->state = STATE_CONNECTED;
                break;
            case PKTTYPE_ERROR:
                repeat = false;
                pHandle->state = STATE_IDLE;
                rv = TLPacket_checkError(ResponseBuf);
                break;
            default:
                //Keep sending
                break;
        }
        if(Timer_checkTimeout(start_time, NORESP_TIMEOUT)){
            printf("Timeout: No Ping Response from performer\n");
            repeat = false;
        }
    }while(repeat);

    return rv;
}
