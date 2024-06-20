#ifndef MCP_H
#define MCP_H

/*
 * TODO: sendSYNC ERROR_BREAK flag seems useless
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"
#include "mcptl.h"
#include "config.h"

typedef struct{
    uint8_t *reg_ids;
    uint8_t *reg_values;
    uint8_t size;
}Register_Payload;

/*
 * COMMAND
 */
typedef struct{
    uint8_t motor_id:3;
    uint16_t command_id:13;
    uint8_t *payload;
    uint8_t payload_size;
}__attribute__((packed)) Command_t;

typedef enum{
    CID_GET_MCP_VERSION         =   0x0000,
    CID_SET_REGISTER            =   0x0001,
    CID_GET_REGISTER            =   0x0002,
    CID_START_MOTOR             =   0x0003,
    CID_STOP_MOTOR              =   0x0004,
    CID_STOP_RAMP               =   0x0005,
    CID_START_STOP              =   0x0006,
    CID_FAULT_ACK               =   0x0007,
}Command_ID;




/*
 * RESPONSE
 */
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




MCPTL_handle *MCP_createHandle(char *port, long baud_rate);
void MCP_freeHandle(MCPTL_handle *pHandle);

int MCP_connect(MCPTL_handle *pHandle);
int MCP_sendCommand(MCPTL_handle *pHandle, uint8_t motor_id, uint8_t command_id, uint8_t *payload, int pl_len);
void MCP_recvResponse(MCPTL_handle *pHandle, Response_t *response);

#endif
