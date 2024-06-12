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



int Services_exec(char *service, char *arguments);

//TODO: static
int Services_CommandParse(char *arguments, Command_t *command);
int Services_RegistryParse(char *arguments);
int Services_DatalogParse(char *arguments);

#endif
