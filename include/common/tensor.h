#include <vector>
#include <iostream>
#include <string>
#include <cstring>

#include "../all_type.h"

int DataTypeSize(DataType type);

std::string DataTypeName(DataType type);

std::string DeviceTypeName(DeviceType type);

class Shape
{
private:
    std::vector<int> shape_;
public:
    Shape(){
        shape_ = std::vector<int>();
    }
    Shape(const std::vector<int>& shape){
        shape_ = shape;
    }
    std::vector<int> GetShape(){
        return shape_;
    }
    int Dim(){
        return shape_.size();
    }
    int Size(){
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
    std::string to_string(){
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
};


class Tensor
{
private:
    std::vector<char> data_;
    Shape shape_;
    DataType data_type_;
    DeviceType device_type_;
public:
    ~Tensor();
    Tensor(){
        data_ = std::vector<char>();
        shape_ = Shape(std::vector<int>());
        data_type_ = Float32;
        device_type_ = CPU;
    }
    Tensor(const Tensor& tensor){
        data_ = tensor.data_;
        shape_ = tensor.shape_;
        data_type_ = tensor.data_type_;
        device_type_ = tensor.device_type_;
    }
    Tensor& operator=(const Tensor& tensor){
        data_ = tensor.data_;
        shape_ = tensor.shape_;
        data_type_ = tensor.data_type_;
        device_type_ = tensor.device_type_;
        return *this;
    }
   
    Tensor(const std::vector<char>& data, const Shape& shape, DataType data_type=Float32, DeviceType device_type=CPU){
        data_ = data;
        shape_ = shape;
        data_type_ = data_type;
        device_type_ = device_type;
    }
    Tensor(void* data_ptr, const Shape& shape, DataType data_type, DeviceType device_type=CPU){
        shape_ = shape;
        data_type_ = data_type;
        device_type_ = device_type;
        data_.resize(shape_.Size() * DataTypeSize(data_type));
        if (data_ptr)
            memcpy(data_.data(), data_ptr, shape_.Size() * DataTypeSize(data_type));
    }
    int Reshape(const Shape& shape){
        shape_ = shape;
        data_.resize(shape_.Size() * DataTypeSize(data_type_));
        return 0;
    }
    int MemSize(){
        return Size() * DataTypeSize(data_type_);
    }
    int Size(){
        return shape_.Size();
    }
    int Dim(){
        return shape_.Dim();
    }
    DataType GetDataType(){
        return data_type_;
    }
    DeviceType GetDeviceType(){
        return device_type_;
    }
    void* GetDataPtr(){
        return data_.data();
    }
    std::vector<int> GetShape(){
        return shape_.GetShape();
    }
    std::string info_to_string(){
        std::string str = "Tensor(";
        str += shape_.to_string();
        str += ", ";
        str += DataTypeName(data_type_);
        str += ", ";
        str += DeviceTypeName(device_type_);
        str += ")";
        return str;
    }
    std::string data_to_string(){
        std::string str = "[";
        for (int i = 0; i < Size(); i++)
        {
            str += std::to_string(data_[i]);
            if (i != Size() - 1)
            {
                str += ", ";
            }
        }
        str += "]";
        return str;
    }

};