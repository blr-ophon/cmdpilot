#include "cmdpilot.h"
#include "mcp.h"

void CMDPilot(int argc, char *argv[]);

int main(int argc, char *argv[]){
    //Parse arguments
    //Execute
    MCPTL_handle *pHandle = MCP_createHandle(
            "/dev/ttyACM0",
            BAUD_RATE 
    );
    //MCP_connect(pHandle);
    test_sendBeacon(pHandle);



    MCP_freeHandle(pHandle);
    //CMDPilot(argc, argv);
    return 0;
}

void handle_set(char *arg) {
    printf("%s\n", arg);
    /*
    char *token = strtok(arg, ",");
    while (token != NULL) {
        char *equal_sign = strchr(token, '=');
        if (equal_sign != NULL) {
            *equal_sign = '\0';
            char *id = token;
            char *value = equal_sign + 1;
            printf("Setting %s to %s\n", id, value);
        }
        token = strtok(NULL, ",");
    }
    */
}

void CMDPilot(int argc, char *argv[]){
    int opt;

    while ((opt = getopt(argc, argv, "R:")) != -1) {
        switch (opt) {
            case 'C': 
                printf("Command\n");
                break;
            case 'R':
                printf("Registry\n");
                //handle_set(optarg);
                break;
            case 'h':
                printf("Help\n");
                break;
            case 'r':
                printf("register list\n");
                break;
            default:
                break;
        }
    }

    //Call Services to execute services 
    
    //Call UI to display Help and reference lists
    
    //Call config to congigure port, baud rate etc
    
    //Call monitor for serial monitoring
    while(0){
        //process input
            //read from shell buffer

        //update
            //execute comands

        //keep-alive
            //
    }
}
