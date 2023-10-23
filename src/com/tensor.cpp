#include "com/tensor.h"
#include "all_type.h"
#include <opencv2/features2d.hpp>

int DataTypeSize(DataType type)
{
    switch (type)
    {
    case Int8:
        return 1;
    case Int64:
        return 8;
    case Float32:
        return 4;
    default:
        return -1;
    }
}

std::string DataTypeName(DataType type)
{
    switch (type)
    {
    case Int8:
        return "Int8";
    case Int64:
        return "Int64";
    case Float32:
        return "Float32";
    default:
        return "Unknown";
    }
}

std::string DeviceTypeName(DeviceType type)
{
    switch (type)
    {
    case CPU:
        return "CPU";
    case CUDA:
        return "CUDA";
    case NPU:
        return "NPU";
    case OTHER:
        return "OTHER";
    default:
        return "Undefined";
    }
}
