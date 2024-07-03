#ifndef MCPTL_H 
#define MCPTL_H 

/*
 * Transport Layer
 */

#include <stdbool.h>
#include "tlpacket.h"
#include "uart.h"
#include "timer.h"

typedef enum{
    CHANNEL_CTRL,
    CHANNEL_SYNC,
    CHANNEL_ASYNC,
}MCPTL_Channel;

typedef enum{
    STATE_IDLE,
    STATE_CONFIGURING,
    STATE_CONNECTING,
    STATE_CONNECTED,
}MCPTL_state;

typedef struct{
    //Synchronous Channel
    uint8_t SYNCrxBuf[256];     
    uint8_t SYNCtxBuf[256];
    //Control Channel
    uint8_t CTRLrxbuf[256];
    uint8_t CTRLtxbuf[256];


    uint8_t state;
    int fd;
    int TXA_Max;    //Controller TXA max. Asynchronous channel
    int TXS_Max;    //Controller TXS max. Synchronous channel
    int RXS_Max;    //Controller RXS max
}MCPTL_handle;

typedef enum{
    RETURN_ERROR    =   1 << 0,     //sendSYNC will return on error instead of resending packet
                                    //Used mainly in connection phase
    NO_TIMEOUT      =   1 << 1,     //sendSYNC will never stop trying to connect 
}sendSYNC_flags;



int MCPTL_stateIDLE(MCPTL_handle *pHandle, const BEACON_t *const LocalBeacon);
int MCPTL_stateCONFIG(MCPTL_handle *pHandle, BEACON_t *LocalBeacon);
int MCPTL_stateCONNECT(MCPTL_handle *pHandle);

void test_sendBeacon(MCPTL_handle *pHandle);

#endif
