#include "registry.h"

CFGRegs_t *Registry_decode(int regToken, uint8_t *regStruct){
    switch(regToken){
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

void Registry_encode(int regToken, uint8_t *regStruct){
    switch(regToken){
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
