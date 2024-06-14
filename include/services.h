#ifndef SERVICES_H
#define SERVICES_H
/*
 * Service Layer
 *
 * Receives the tokenized string. Parses and sends to performer
 * via transport layer
 */


#include "error.h"
#include "mcp.h"
#include "mcptl.h"

int Services_Handler(char *service, char *arguments);

#endif
