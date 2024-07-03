#include "uart.h"

/*
 * Given the name of a serial port and a Baud Rate
 * returns a file descriptor
 */
int UART_getfd(char *port, int baud_rate){
    int fd;
    struct termios options;

    fd = open(port, O_RDWR | O_NDELAY | O_NOCTTY);
    if(fd < 0){
        printf("Error opening serial port\n");
        perror("open");
        fd = -1;
        goto out;
    }else{
        printf("Serial Port: %s\n", port);
        printf("Baud Rate: %d\n", baud_rate);
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


/*
 * Writes n bytes from tx_buf on fd
 */
int UART_Send(int fd, uint8_t *tx_buf, int n){
    int rv = 0;
    rv = write(fd, tx_buf, n);
    if(rv < 0){
        perror("write");
    }
    return rv;
}

/*
 * Reads from fd to rx_buf
 */
int UART_Recv(int fd, uint8_t *rx_buf, int buf_size){
    int rv = 0;
    rv = read(fd, rx_buf, buf_size);
    if(rv < 0){
        perror("read");
    }
    return rv;
}

