#include "cmdpilot.h"
#include "mcp.h"

int main(int argc, char *argv[]){
    //Parse arguments
    //Execute
    MCPTL_handle *pHandle = MCP_createHandle(
            "/dev/tty0",
            BAUD_RATE 
    );
    MCP_connect(pHandle);

    MCP_freeHandle(pHandle);
    return 0;
}

void CMDPilot(int argc, char *argv[]){
    //Call Services to execute services 
    
    //Call UI to display Help and reference lists
    
    //Call config to congigure port, baud rate etc
    
    //Call monitor for serial monitoring
    while(1){
        //process input
            //read from shell buffer

        //update
            //execute comands

        //keep-alive
            //
    }
}
