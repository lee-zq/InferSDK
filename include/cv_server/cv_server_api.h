#pragma once

#include "message.h"

// TODO 添加接口说明
int CreateCVServer(void** inst);

int DestroyCVServer(void* CVServer);

int ProcessCVServer(void* CVServer, message* msg);

int StartCVServer(void* CVServer);

int StopCVServer(void* CVServer);

int InitCVServer(void* CVServer, const char* cfg);

int UninitCVServer(void* CVServer);
