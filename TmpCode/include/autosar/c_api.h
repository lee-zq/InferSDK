#pragma once

#include "message.h"

// TODO 添加接口说明
int CreateAutosar(void** inst);
int DestroyAutosar(void* Autosar);
int ProcessAutosar(void* Autosar, message* msg);
int StartAutosar(void* Autosar);
int StopAutosar(void* Autosar);
int SnitAutosar(void* Autosar);
int UninitAutosar(void* Autosar);




