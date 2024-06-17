#include "uart.h"


int UART_getfd(char *port, int baud_rate){
    int fd;
    struct termios options;

    fd = open(port, O_RDWR | O_NDELAY | O_NOCTTY);
    if(fd < 0){
        perror("Error opening serial port");
        goto out;
    }

    //Set up serial port
    
    //tcgetattr(fd, &options);
    options.c_cflag = baud_rate | CS8 | CLOCAL | CREAD;
    //cfsetspeed(&options, baud_rate);
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    //Apply settings
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

out:
    return fd;
}


int UART_Send(int fd, uint8_t *tx_buf, int n){
    int rv = 0;
    rv = write(fd, tx_buf, n);
    return rv;
}

int UART_Recv(int fd, uint8_t *rx_buf, int buf_size){
    int rv = 0;
    rv = read(fd, rx_buf, buf_size);
    return rv;
}

