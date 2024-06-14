#include "services.h"

//Local functions
static int Services_CommandParse(char *arguments, Command_t *command);
static int Services_RegistryParse(char *arguments);
static int Services_DatalogParse(char *arguments);

/*
 * Format:
 * cmdpilot -c <command> <arguments> <flags>
 */


/*
 * Service: single character specifying the type of service (command, registry, datalog)
 * Arguments: Arguments for the service
 * Ex.: cmdpilot -c startmotor. C is the service(command) and startmotor the argument
 */
int Services_Handler(char *service, char *arguments){
    int rv = 0;
    char serv = service[0];

    switch(serv){
        case 'C':   //Command
            { //TODO If the block becomes too big, copy it into a command_handler function
            //Parse command
            Command_t command;
            Services_CommandParse(arguments, &command);

            //Serialize and send command
            //Serialize_Command(&command, buf);
            //MCPTL_send(buf)
            
            //Poll for responses
            //MCPTL_recv(buf)
            break;
            }

        case 'R':   //Registry
            break;
        case 'D':   //Datalog
            /*
             * Not Implemented
             */
            break;
        default:
            rv |= -EINVARG;
            break;
    }

    return rv;
}


/* Parse Functions */

static int Services_CommandParse(char *arguments, Command_t *command){
    //Parse all arguments to command struct

    return 0;
}

static int Services_RegistryParse(char *arguments){
    //Parse all arguments to a struct
    return 0;
}

static int Services_DatalogParse(char *arguments){
    //Parse all arguments to a struct
    return 0;
}


/* Handler Functions */

int Services_CommandHandler(Command_t *command){
    return 0;
}
