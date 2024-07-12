#include "cmdpilot.h"

void CMDPilot(int argc, char *argv[]);
void cmdpilot_main(void);

int main(int argc, char *argv[]){
    //CMDPilot(argc, argv);
    cmdpilot_main();
    return 0;



}


void cmdpilot_main(void){

    UI_displayHelp();
    char buf[64] = {0}; 
    MCPTL_handle *pHandle = NULL;

    while(1){
        char state_str[12] = "IDLE";
        if(pHandle){
            switch(pHandle->state){
                case STATE_IDLE:
                    strncpy(state_str, "IDLE", 12);
                    break;
                case STATE_CONNECTED:
                    strncpy(state_str, "CONNECTED", 12);
                    break;
            }
        }
        printf("[%s]>> ", state_str);
        fgets(buf, 31, stdin);
        buf[strcspn(buf, "\n")] = 0;
        
        //TODO
        //Tokenize buf and deal only with first token
        //parse other tokens into arguments

        if(strcmp(buf, "exit") == 0){
            break;
        }
        else if(strcmp(buf, "help") == 0){
            UI_displayHelp();
        }
        else if(strcmp(buf, "connect") == 0){
            cmdpilot_connect(pHandle);
        }
        else if(strcmp(buf, "command") == 0){
            printf("command\n");
        }
        else if(strcmp(buf, "register") == 0){
        }
        else if(strcmp(buf, "datalog") == 0){
        }
        else if(strcmp(buf, "reglist") == 0){
            UI_displayRegList();
        }
        else{
            printf("Invalid option\n");
        }

        //fputs(buf, stdout);
    }

    MCP_freeHandle(pHandle);
}



int cmdpilot_connect(MCPTL_handle *pHandle){
    int rv = 0;

    pHandle = MCP_createHandle(
            "/dev/ttyACM0",
            BAUD_RATE 
    );
    if(!pHandle){
        printf("Error creating Handle\n");
        rv = -1;
        goto out;
    }
    if(MCP_connect(pHandle) < 0){
        rv = -1;
        goto out;
    }

out:
    return rv;
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
                //Call services.c functions to execute Command services 
                printf("Command\n");
                break;
            case 'R':
                //Call services.c to execute Registry services 
                printf("Registry\n");
                //handle_set(optarg);
                break;
            case 'h':
                //Call ui.c functions to display Help 
                printf("Help\n");
                break;
            case 'r':
                //Call ui.c functions to display reference lists
                printf("register list\n");
                break;
            default:
                break;
        }
    }

    //Shell mode
    while(0){
        //process input
            //read from shell buffer

        //update
            //execute comands

        //keep-alive
            //
    }
}
