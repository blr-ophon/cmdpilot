#ifndef MCPTL_H 
#define MCPTL_H 

/*
 * Transport Layer
 */

#include "mcp.h"

int MCPTL_socket(void);

int MCPTL_connect(void);

int MCPTL_listen(void);

int MCPTL_send(uint8_t *buf);

int MCPTL_recv(void);



#endif
