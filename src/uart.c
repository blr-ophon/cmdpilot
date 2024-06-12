#include "uart.h"


/*
 * This function monitors a serial port. Sends a byte stream to it
 * and waits for a response for a given timeout. 
 * It returns errors if its unable to write/read given serial port or
 * if there was no response.
 */
int USART_Send(char *port, uint8_t *tx_buf, int buf_size){
    //Error if no response during limited time
    return 0;
}

int USART_Recv(char *port, uint8_t *rx_buf, int buf_size){
    //Error if no response during limited time
    return 0;
}

