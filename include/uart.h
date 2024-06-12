#ifndef UART_H
#define UART_H

#include <stdint.h>


int USART_Send(char *port, uint8_t *tx_buf, int buf_size);

int USART_Recv(char *port, uint8_t *rx_buf, int buf_size);


#endif
