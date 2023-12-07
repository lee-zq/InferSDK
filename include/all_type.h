#pragma once
#include "base_type.h"
#include "com/define.h"
#include "cv_server/message.h"
#include <string>
#include <vector>

SPACE_BEGIN
typedef enum DeviceType
{
    CPU,
    CUDA,
    NPU,
    OTHER
} DeviceType;

typedef enum DataType
{
    Unknown,
    Int8,
    Int32,
    Int64,
    Float32,
} DataType;

typedef struct _ModuleParamType
{
    std::string type;
    std::string res_path;
    DeviceType dev_type;
    int dev_id;
    int thread_num;
} ModuleParamType;

typedef struct _InstParamType
{
    std::string type;
    int thread_num;
    int dev_type;
    int dev_id;
    std::vector<std::pair<ModuleType, ModuleParamType>> module_params;
} InstParamType;

SPACE_END