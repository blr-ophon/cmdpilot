#ifndef REGISTRY_H
#define REGISTRY_H

#include <stdint.h>

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

typedef struct{
    uint32_t MCSDKVersion;
    uint8_t MotorNumber;
    uint8_t MCP_Flag;
    uint8_t MCP_UARTA_Log;
    uint8_t MCP_UARTB_Log;
    uint8_t MCP_STLINK_Log;
}Global_config_t;

typedef struct{
    uint32_t PolePairs;
    uint32_t RatedFlux;
    uint32_t Rs;
    uint32_t RsSkinFactor;
    uint32_t Ls;
    uint32_t Ld;
    uint32_t MaxCurrent;
    uint32_t CopperMass;
    uint32_t CoolingTau;
    uint32_t Name;
}Motor_config_t;

typedef struct{
    uint32_t MaxMechSpeed;
    uint32_t MaxReadableCurrent;
    uint16_t nominalCurrent;
    uint16_t nominalVoltage;
    uint8_t DriveType;
    uint8_t Padding;
}Application_config_t;

typedef struct{
    uint8_t PrimSpeedPosSensingType; 
    uint8_t AuxSpeedPosSensingType; 
    uint8_t CurrentSensingTopology; 
    uint8_t FOCRate; 
    uint32_t PWMFrequency;
    uint16_t MediumFrequencyTaskFreq;
    uint16_t ConfigurationFlags1;
    uint16_t ConfigurationFlags2;
}FOC_firmware_cfg_t;

typedef struct{
    uint8_t PrimSpeedPosSensingType; 
    uint8_t CurrentSensingTopology; 
    uint32_t PWMFrequency;
    uint16_t MediumFrequencyTaskFreq;
    uint16_t ConfigurationFlags1;
}Sixstep_firmware_cfg_t;

typedef struct{
    uint8_t HFRate;
    uint8_t HFNum;
    uint8_t MFRate;
    uint8_t MFNum;
    uint8_t *HFID;
    uint8_t *MFID;
    uint8_t Mark;
}Datalog_regs_t;

typedef union{
    Global_config_t globalCFG;
    Motor_config_t motorCFG;
    Application_config_t appCFG;
    FOC_firmware_cfg_t focFirmCFG;
    FOC_firmware_cfg_t sixstepFirmCFG;
    Datalog_regs_t datalogCFG;
}CFGRegs_t;


#endif
