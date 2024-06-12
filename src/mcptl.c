#include "mcptl.h"


/*
 * Creates an MCPTL socket that contains two independent channels
 */
int MCPTL_socket(void){
    return 0;
}

/*
 * 
 */
int MCPTL_connect(void){
    return 0;
}

int MCPTL_send(uint8_t *buf){
    //Send Header
    //Send Payload
    //Send CRC
    
    //Wait for an acknowledge
    return 0;
}

int MCPTL_recv(void){
    //Receive Header
    //Receive Payload
    //Receive CRC
    
    //If bad CRC, send a packet notifying
    //bad CRC and wait for new packet
    return 0;
}

