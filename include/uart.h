#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "error.h"


int UART_getfd(char *port, int baud_rate);

int UART_Send(int fd, uint8_t *tx_buf, int n);

int UART_Recv(int fd, uint8_t *rx_buf, int buf_size);

#endif
