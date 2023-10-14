#pragma once
#include <string>

typedef enum DeviceType{
    CPU,
    CUDA,
    NPU, 
    OTHER
} DeviceType;

typedef enum DataType{
    Int8,
    Float32,
    Double64,
    Float16
} DataType;

typedef struct InferEngineParam
{
   std::string onnx_path;
   DeviceType dev_type;
   int dev_id;
   int thread_num;
} InferEngineParam;

