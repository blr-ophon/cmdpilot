#include "command.h"

/*
 * Strings to structs
 * Call serialization of structs
 */


/*
 * Receives parsed command and arguments
 */
int Command_Execute(char *Command, char *Arguments){
    int rv = 0;
    //Command ID
    char *endptr;
    uint8_t cid = strtol(Command, &endptr, 10); 
    if(*endptr != '\0'){
        //TODO: Not a valid integer
        rv = -EINVCOMMAND;
        goto out;
    }

    switch(cid){
        case CID_GET_MCP_VERSION:
            break;
        case CID_SET_REGISTER:            
            break;
        case CID_GET_REGISTER:           
            break;
        default:
            rv = -EINVCOMMAND;
            goto out;
    }
out:
    return rv;
}

int Command_GetMCPVersion(char *arguments){
    //Parse arguments
    //Call MCP_GetMCPVersion
    //Convert response to string and print
    return 0;
}

int Command_SetRegister(uint8_t *payload, int n){
    //Send command
    //Read response
        //Error handling
    return 0;
}

int Command_GetRegister(uint8_t *payload, uint8_t *response, int n){
    //Send command
    //Read response
        //Error handling
    return 0;
}
