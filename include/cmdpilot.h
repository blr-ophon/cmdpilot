#ifndef CMDPILOT_H
#define CMDPILOT_H

#include "services.h"


typedef enum{
    SVC_COMMAND,
    SVC_REGISTER,
    SVC_DATALOG,
    SVC_HELP,
    SVC_REGLIST,
}CMDPILOT_SERVICES;


typedef struct{
    int svc;
    char *arg1;
    char *arg2;
    //int flags;
}parsedServices_t;

void cmdpilot_help(void);
int cmdpilot_connect(MCPTL_handle *pHandle);

#endif
