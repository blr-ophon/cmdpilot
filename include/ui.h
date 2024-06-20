#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdint.h>

void displayRegisterList(void);
void displayHelp(void);

typedef enum{
    REG_GLOBAL_CONFIG,            
    REG_MOTOR_CONFIG,             
    REG_APPLICATION_CONFIG,       
    REG_FOC_FIRMWARE_CFG,        
    REG_SIXSTEP_FIRMWARE_CFG,   
    REG_DATALOG_UARTA,         
    REG_DATALOG_UARTB,        
    REG_DATALOG_STLINK,      
    REG_SHIFT_UARTA,        
    REG_SHIFT_UARTB,       
    REG_SHIFT_STLINK
}Registers;


#endif
