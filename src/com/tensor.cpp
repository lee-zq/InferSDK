#include "com/tensor.h"
#include "all_type.h"
#include <opencv2/features2d.hpp>

int DataTypeSize(DataType type)
{
    switch (type)
    {
    case Int8:
        return 1;
    case Float32:
        return 4;
    case Double64:
        return 8;
    case Float16:
        return 2;
    default:
        return 0;
    }
}

std::string DataTypeName(DataType type)
{
    switch (type)
    {
    case Int8:
        return "Int8";
    case Float32:
        return "Float32";
    case Double64:
        return "Double64";
    case Float16:
        return "Float16";
    default:
        return "Undefined";
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
