#ifndef MCPAPI_H
#define MCPAPI_H

#include "mcptl.h"


int MCPTL_connect(MCPTL_handle *pHandle);
int MCPTL_sendCommand(MCPTL_handle *pHandle, uint8_t command, uint8_t *payload, int pl_len);

#endif
