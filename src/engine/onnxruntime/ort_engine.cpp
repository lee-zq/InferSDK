#include "ort_engine.h"
#include "onnxruntime_c_api.h"
#include "onnxruntime_cxx_api.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <time.h>
#include <typeindex>
#include <vector>

using namespace std;

int ORTEngine::init(const InferEngineParam& param)
{
    param_ = param;
    memory_info_ =
        Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    session_option.SetGraphOptimizationLevel(
        GraphOptimizationLevel::ORT_ENABLE_ALL);
    session_option.SetExecutionMode(ExecutionMode::ORT_PARALLEL);
    session_option.SetIntraOpNumThreads(param_.thread_num);
    if (param_.dev_type == CUDA)
    {
        OrtCUDAProviderOptions cuda_options;
        session_option.AppendExecutionProvider_CUDA(cuda_options);
    }
    session_ = new Ort::Session(env, param_.onnx_path.c_str(), session_option);
    if (session_ == nullptr)
    {
        printf("Ort::AllocatorWithDefaultOptions allocator is nullptr\n");
        return -1;
    }

    Ort::AllocatorWithDefaultOptions* allocator_ =
        new Ort::AllocatorWithDefaultOptions();
    if (allocator_ == nullptr)
    {
        printf("Ort::AllocatorWithDefaultOptions allocator is nullptr\n");
        return -1;
    }

    input_num_ = session_->GetInputCount();
    output_num_ = session_->GetOutputCount();
    for (int i = 0; i < input_num_; i++)
    {
        input_names_.push_back(session_->GetInputName(i, *allocator_));
    }

    for (int i = 0; i < output_num_; i++)
    {
        output_names_.push_back(session_->GetOutputName(i, *allocator_));
    }
    for (int i = 0; i < input_num_; i++)
    {
        input_shapes_.push_back(session_->GetInputTypeInfo(i)
                                    .GetTensorTypeAndShapeInfo()
                                    .GetShape());
    }
    for (int i = 0; i < output_num_; i++)
    {
        output_shapes_.push_back(session_->GetOutputTypeInfo(i)
                                     .GetTensorTypeAndShapeInfo()
                                     .GetShape());
    }

    input_tensor_.clear();
    output_tensor_.clear();
    return 0;
}

int ORTEngine::forward(const vector<Tensor>& input_data,
                       vector<Tensor>& output_data)
{
    if (input_data.size() != input_num_)
    {
        printf("input_data.size() != input_num_\n");
        return -1;
    }
    input_tensor_.clear();
    for (int i = 0; i < input_num_; i++)
    {
        Tensor& cur_input = const_cast<Tensor&>(input_data[i]);
        input_tensor_.emplace_back(CreateOrtValueFromTensor(cur_input));
    }

    try
    {
        output_tensor_ = session_->Run(Ort::RunOptions{nullptr},
                                       input_names_.data(),
                                       input_tensor_.data(),
                                       input_names_.size(),
                                       output_names_.data(),
                                       output_names_.size());
    }
    catch (const std::exception& ex)
    {
        printf("exception in Ort::Session_->Run: %s\n", ex.what());
        return -1;
    }
    if (output_tensor_.size() != output_num_)
    {
        printf("output_tensor_.size() != output_num_\n");
        return -1;
    }

    for (int i = 0; i < output_num_; i++)
    {
        Ort::Value& cur_value = output_tensor_[i];
        Tensor& cur_output = output_data[i];
        int ret = CopyOrtValue2Tensor(cur_value, cur_output);
    }

    return 0;
}

Ort::Value ORTEngine::CreateOrtValueFromTensor(Tensor& tensor)
{
    vector<int64_t> shape_data = tensor.GetShape().GetDataInt64();
    DataType type = tensor.GetDataType();
    switch (type)
    {
    case Float32:
        return Ort::Value::CreateTensor<float>(memory_info_,
                                               (float*)tensor.GetDataPtr(),
                                               tensor.Size(),
                                               shape_data.data(),
                                               shape_data.size());
        break;
    case Int8:
        return Ort::Value::CreateTensor<int8_t>(memory_info_,
                                                (int8_t*)tensor.GetDataPtr(),
                                                tensor.Size(),
                                                shape_data.data(),
                                                shape_data.size());
        break;
    case Int64:
        return Ort::Value::CreateTensor<int64_t>(memory_info_,
                                                 (int64_t*)tensor.GetDataPtr(),
                                                 tensor.Size(),
                                                 shape_data.data(),
                                                 shape_data.size());
        break;
    default:
        printf("Tensor2OrtValue: unsupported data type: %s\n",
               DataTypeName(type).c_str());
        return Ort::Value::CreateTensor<float>(nullptr, nullptr, 0, nullptr, 0);
    }
}

int ORTEngine::CopyOrtValue2Tensor(Ort::Value& value, Tensor& tensor)
{
    auto value_info = value.GetTensorTypeAndShapeInfo();
    vector<int64_t> value_shape = value_info.GetShape();
    ONNXTensorElementDataType value_type = value_info.GetElementType();
    switch (value_type)
    {
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT:
        tensor.Reshape(value_shape, Float32);
        memcpy(tensor.GetDataPtr(),
               value.GetTensorMutableData<float>(),
               tensor.MemSize());
        break;

    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8:
        tensor.Reshape(value_shape, Int8);
        memcpy(tensor.GetDataPtr(),
               value.GetTensorMutableData<int8_t>(),
               tensor.MemSize());
        break;

    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64:
        tensor.Reshape(value_shape, Int64);
        memcpy(tensor.GetDataPtr(),
               value.GetTensorMutableData<int64_t>(),
               tensor.MemSize());
        break;

    default:
        printf("OrtValue2Tensor: unsupported data type\n");
        return -1;
    }
    return 0;
}