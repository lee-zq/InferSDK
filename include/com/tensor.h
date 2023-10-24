#pragma once
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "../all_type.h"

inline int DataTypeSize(DataType type)
{
    switch (type)
    {
    case Int8:
        return 1;
    case Int32:
        return 4;
    case Int64:
        return 8;
    case Float32:
        return 4;
    default:
        return -1;
    }
}

inline std::string DataTypeName(DataType type)
{
    switch (type)
    {
    case Int8:
        return "Int8";
    case Int32:
        return "Int32";
    case Int64:
        return "Int64";
    case Float32:
        return "Float32";
    default:
        return "Unknown";
    }
}

inline std::string DeviceTypeName(DeviceType type)
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


class Shape
{
private:
    std::vector<int> shape_;

public:
    Shape()
    {
        shape_ = std::vector<int>();
    }
    Shape(const std::vector<int>& shape)
    {
        shape_ = shape;
    }
    Shape(const std::vector<int64_t>& shape)
    {
        shape_ = std::vector<int>();
        for (int i = 0; i < shape.size(); i++)
        {
            shape_.push_back(static_cast<int>(shape[i]));
        }
    }

    Shape(const Shape& shape)
    {
        shape_ = shape.shape_;
    }
    int& operator[](int index)
    {
        return shape_[index];
    }
    Shape& operator=(const Shape& shape)
    {
        shape_ = shape.shape_;
        return *this;
    }
    int Reshape(const std::vector<int>& shape)
    {
        shape_ = shape;
        return 0;
    }
    const std::vector<int>& GetData()
    {
        return shape_;
    }
    std::vector<int64_t> GetDataInt64() const
    {
        std::vector<int64_t> shape(shape_.size());
        for (int i = 0; i < shape_.size(); i++)
        {
            int64_t tmp = static_cast<int64_t>(shape_[i]);
            shape[i] = tmp;
        }
        return shape;
    }
    int Dim()
    {
        return shape_.size();
    }
    int Size()
    {
        if (shape_.size() == 0)
        {
            return 0;
        }
        int size = 1;
        for (int i = 0; i < shape_.size(); i++)
        {
            size *= shape_[i];
        }
        return size;
    }
    std::string to_string()
    {
        std::string str = "(";
        for (int i = 0; i < shape_.size(); i++)
        {
            str += std::to_string(shape_[i]);
            if (i != shape_.size() - 1)
            {
                str += ", ";
            }
        }
        str += ")";
        return str;
    }
    int stride(int index)
    {
        if (index < 0 || index >= shape_.size())
        {
            std::cout << "Shape index out of range" << std::endl;
            return -1;
        }
        int stride = 1;
        for (int i = index + 1; i < shape_.size(); i++)
        {
            stride *= shape_[i];
        }
        return stride;
    }
};


class Tensor
{
private:
    std::vector<char> data_;
    Shape shape_;
    DataType data_type_;
    DeviceType device_type_;

public:
    ~Tensor(){};
    Tensor()
    {
        data_type_ = Float32;
        device_type_ = CPU;
    }
    Tensor(const Tensor& tensor)
    {
        data_ = tensor.data_;
        shape_ = tensor.shape_;
        data_type_ = tensor.data_type_;
        device_type_ = tensor.device_type_;
    }
    Tensor& operator=(const Tensor& tensor)
    {
        data_ = tensor.data_;
        shape_ = tensor.shape_;
        data_type_ = tensor.data_type_;
        device_type_ = tensor.device_type_;
        return *this;
    }
    Tensor operator[](int index)
    {
        // Create sub Tensor from first dim with deepcopy
        if (index<0 || shape_.Dim()==0 || shape_.GetData().front()<index)
        {
            std::cout << "Tensor::operator[], Index out of range" << std::endl;
            return Tensor();
        }
        Shape shape;
        if (shape_.Dim() == 1)
        {
            shape = Shape(std::vector<int>{1});
        }
        else
        {
            shape = std::vector<int>(shape_.GetData().begin() + 1,
                                     shape_.GetData().end());
        }
        Tensor tensor(data_.data() + index * shape_.stride(0) * DataTypeSize(data_type_),
                        shape,
                        data_type_,
                        device_type_);
        return tensor;
    }
    Tensor(const std::vector<char>& data,
           const Shape& shape,
           DataType data_type = Float32,
           DeviceType device_type = CPU)
    {
        data_ = data;
        shape_ = shape;
        data_type_ = data_type;
        device_type_ = device_type;
    }
    Tensor(void* data_ptr,
           const Shape& shape,
           DataType data_type,
           DeviceType device_type = CPU)
    {
        shape_ = shape;
        data_type_ = data_type;
        device_type_ = device_type;
        data_.resize(shape_.Size() * DataTypeSize(data_type));
        if (data_ptr)
            memcpy(data_.data(),
                   data_ptr,
                   shape_.Size() * DataTypeSize(data_type));
    }
    int Reshape(const Shape& shape, DataType data_type = Float32)
    {
        shape_ = shape;
        data_type_ = data_type;
        data_.resize(shape_.Size() * DataTypeSize(data_type_));
        return 0;
    }
    int MemSize()
    {
        return Size() * DataTypeSize(data_type_);
    }
    int Size()
    {
        return shape_.Size();
    }

    int Dim()
    {
        return shape_.Dim();
    }
    DataType GetDataType()
    {
        return data_type_;
    }
    DeviceType GetDeviceType()
    {
        return device_type_;
    }
    void* GetDataPtr()
    {
        return data_.data();
    }
    const Shape& GetShape()
    {
        return shape_;
    }
    template<typename T>
    T item()
    {
        if (shape_.Size() != 1)
        {
            std::cout << "Tensor shape is not 1" << std::endl;
            return 0;
        }
        switch (data_type_)
        {
        case Float32:
            return *(reinterpret_cast<float*>(data_.data()));
        case Int32:
            return *(reinterpret_cast<int*>(data_.data()));
        case Int8:
            return *(reinterpret_cast<int8_t*>(data_.data()));
        case Int64:
            return *(reinterpret_cast<int64_t*>(data_.data()));
        default:
            std::cout << "Tensor data type is not supported" << std::endl;
            return 0;
        }
    }
    template <typename T>
    std::vector<T> dump_to_vector()
    {
        T* data_ptr = reinterpret_cast<T*>(data_.data());
        std::vector<T> vec(data_ptr, data_ptr + shape_.Size());
        return vec;
    }
    std::string info_to_string()
    {
        std::string str = "Tensor(";
        str += shape_.to_string();
        str += ", ";
        str += DataTypeName(data_type_);
        str += ", ";
        str += DeviceTypeName(device_type_);
        str += ")";
        return str;
    }
    int dump_to_file(const std::string& file_path)
    {
        FILE* fp = fopen(file_path.c_str(), "wb");
        if (fp == nullptr)
        {
            std::cout << "open file error" << std::endl;
            return -1;
        }
        float* data_ptr = reinterpret_cast<float*>(data_.data());
        for (int i = 0; i < shape_.Size(); i++)
        {
            fprintf(fp, "%f\n", data_ptr[i]);
        }
        fclose(fp);
        return 0;
    }

    int load_from_file(const std::string& file_path, const Shape& shape)
    {
        Reshape(shape);
        FILE* fp = fopen(file_path.c_str(), "rb");
        if (fp == nullptr)
        {
            std::cout << "open file error" << std::endl;
            return -1;
        }
        float* data_ptr = reinterpret_cast<float*>(data_.data());
        for (int i = 0; i < shape_.Size(); i++)
        {
            fscanf(fp, "%f\n", &data_ptr[i]);
        }
        fclose(fp);
        return 0;
    }
};