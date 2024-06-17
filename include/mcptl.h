#ifndef MCPTL_H 
#define MCPTL_H 

/*
 * Transport Layer
 */

#include "mcp.h"
#include "packet.h"
#include "serialize.h"

typedef enum{
    STATE_IDLE,
    STATE_CONFIGURING,
    STATE_CONNECTING,
    STATE_CONNECTED,
}MCPTL_state;

typedef struct{
    uint8_t txBuf[64];
    uint8_t rxBuf[64];
    uint8_t state;
}MCPTL_handle;


MCPTL_handle *MCPTL_getHandle(void);

int MCPTL_connect(MCPTL_handle *pHandle);

int MCPTL_send(MCPTL_handle *pHandle, uint8_t *buf, int *n);

int MCPTL_recv(MCPTL_handle *pHandle, uint8_t *buf, int *n);



#endif
