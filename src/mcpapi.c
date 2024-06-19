#include "mcpapi.h"


int MCPTL_connect(MCPTL_handle *pHandle){
    //TODO: if any of the received packets is ERROR,
    //restart connection from the beginning
    BEACON_t LocalBeacon;
    Beacon_set(&LocalBeacon,0,0,0,0,0,0);
    BEACON_t PerformerBeacon;

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
                MCPTL_connect(pHandle);
                break;
            default:
                break;
                //TODO: Bad packet / Handle
        }
    }
    return 0;
}

int MCPTL_sendCommand(MCPTL_handle *pHandle, uint8_t command, uint8_t *payload, int pl_len){
    //Create command packet with specified command id
    //sendSYNC
    //decode response and return it
    return 0;
}

int MCPTL_Datalog(MCPTL_handle *pHandle, uint8_t *payload, int pl_len){
    //Create command packet with specified command id
    //sendSYNC
    //decode response and return it
    return 0;
}
