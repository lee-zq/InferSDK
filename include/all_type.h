#pragma once
#include <string>

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
    Int64,
    Float32,
} DataType;

typedef struct InferEngineParam
{
    std::string onnx_path;
    DeviceType dev_type;
    int dev_id;
    int thread_num;
} InferEngineParam;

typedef struct ModuleParam
{
    std::string type;
    // 此处添加Module构造函数传入的结构体参数
} ModuleParam;
