#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>
#include <stdint.h>

#include "mcp.h"
#include "error.h"



int Command_Execute(char *Command, char *Arguments);

//TODO: static
int Command_GetMCPVersion(char *arguments);
int Command_SetRegister(uint8_t *payload, int n);
int Command_GetRegister(uint8_t *payload, uint8_t *response, int n);

#endif
