#ifndef MCP_H
#define MCP_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

typedef struct{
    uint8_t motor_id:3;
    uint16_t command_id:13;
    uint8_t *payload;
}MCP_command_t;

typedef struct{
    uint8_t *reg_ids;
    uint8_t *reg_values;
    uint8_t size;
}Register_Payload;

typedef struct{
    uint8_t *payload;
    uint8_t payload_size;
    uint8_t status_code;
}Response_t;





typedef enum{
  RP_CMD_OK                =   0x00,
  RP_CMD_NOK               =   0x01,
  RP_CMD_UNKNOWN           =   0x02,
  RP_RO_REG                =   0x04,
  RP_UNKNOWN_REG           =   0x05,
  RP_STRING_FORMAT         =   0x06,
  RP_BAD_DATA_TYPE         =   0x07,
  RP_NO_TXSYNC_SPACE       =   0x08,
  RP_NO_TXASYNC_SPACE      =   0x09,
  RP_WRONG_STRUCT_FORMAT   =   0x0a,
  RP_WO_REG                =   0x0b,
  RP_USER_CMD_NOT_IMPL     =   0x0d,
}Response_Code;

typedef enum{
    CID_GET_MCP_VERSION         =   0x0000,
    CID_SET_REGISTER            =   0x0001,
    CID_GET_REGISTER            =   0x0002,
}Command_ID;


Response_t TPL_SendCommand(uint8_t *Packet);

#endif
