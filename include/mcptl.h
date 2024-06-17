#ifndef MCPTL_H 
#define MCPTL_H 

/*
 * Transport Layer
 */

#include <stdbool.h>
#include "mcp.h"
#include "packet.h"
#include "serialize.h"
#include "uart.h"

typedef enum{
    STATE_IDLE,
    STATE_CONFIGURING,
    STATE_CONNECTING,
    STATE_CONNECTED,
}MCPTL_state;

typedef struct{
    uint8_t txBuf[256];
    uint8_t rxBuf[256];
    uint8_t state;
    int fd;
    int TXA_Max;    //Controller TXA max. Asynchronous channel
    int TXS_Max;    //Controller TXS max. Synchronous channel
    int RXS_Max;    //Controller RXS max
}MCPTL_handle;


MCPTL_handle *MCPTL_getHandle(void);

int MCPTL_connect(MCPTL_handle *pHandle);

int MCPTL_sendSYNC(MCPTL_handle *pHandle, int *n);
int MCPTL_sendASYNC(MCPTL_handle *pHandle, int *n);

#endif
