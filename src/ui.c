#include "ui.h"

#define REG_NUMBER 11

char *names[] = {
    "GLOBAL_CONFIG",            
    "MOTOR_CONFIG",             
    "APPLICATION_CONFIG",       
    "FOC_FIRMWARE_CFG",        
    "SIXSTEP_FIRMWARE_CFG",   
    "DATALOG_UARTA",         
    "DATALOG_UARTB",        
    "DATALOG_STLINK",      
    "SHIFT_UARTA",        
    "SHIFT_UARTB",       
    "SHIFT_STLINK",
};

char *Id_Tokens[] = {
    "0x28",
    "0x69 - 0x6f",
    "0xa9 - 0xaf",
    "0xe9 - 0xef",
    "0xe9 - 0xef",
    "0x528",
    "0x568",
    "0x5a8",
    "0x668",
    "0x6a8",
    "0x6e8",
};

/*
 * Show the list of available registers and their IDS
 */
void UI_displayRegList(void){
    for(int i = 0; i < REG_NUMBER; i++){
        printf("%s: %s\n", names[i], Id_Tokens[i]);
    }
    printf("\n");
}

/*
 * Displays additional info for every register struct
 */
void UI_registerInfo(int reg, uint8_t *regStruct){
    switch(reg){
        case REG_GLOBAL_CONFIG:
            break;
        case REG_MOTOR_CONFIG:
            break;
        case REG_APPLICATION_CONFIG:
            break;
        case REG_FOC_FIRMWARE_CFG:
            break;
        case REG_SIXSTEP_FIRMWARE_CFG:   
            break;
        case REG_DATALOG_UARTA:
            break;
        case REG_DATALOG_UARTB:
            break;
        case REG_DATALOG_STLINK:
            break;
        case REG_SHIFT_UARTA:
            break;
        case REG_SHIFT_UARTB:
            break;
        case REG_SHIFT_STLINK:
            break;
    }
}

void UI_displayHelp(void){
    printf("OPTIONS:\n"
            "   \nconnect: Connect to performer\n"
            "   \ncommand <mcp_command>\n"
            "       mcp_command: getmcpversion, startmotor, stopmotor, stopramp, startstop, faultack\n"
            "   \nregister <set/get> <register(s)>\n"
            "       register set (reg1_id=val1,reg2_id=val2,...)\n"
            "       register get <reg_id>\n"
            "   \nreglist: Display register list\n"
            "   \nhelp: Display help\n"
            "   \nexit: Exit application\n\n"
    );
}

