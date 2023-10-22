#include "ort_engine.h"
#include "onnxruntime_c_api.h"
#include "onnxruntime_cxx_api.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <time.h>
#include <typeindex>
#include <vector>

using namespace std;

static ONNXTensorElementDataType GetONNXType(const DataType& type)
{
    switch (type)
    {
    case Float32:
        return ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
    case Float16:
        return ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16;
    case Int8:
        return ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8;
    case Double64:
        return ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX64;
    default:
        return ONNX_TENSOR_ELEMENT_DATA_TYPE_UNDEFINED;
    }
}

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

    Ort::AllocatorWithDefaultOptions *allocator_ =
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
        input_names_.push_back(session_->GetInputName(i, *allocator_));
    }

    for (int i = 0; i < output_num_; i++)
    {
        output_names_.push_back(session_->GetOutputName(i, *allocator_));
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

int ORTEngine::forward(const vector<Tensor> &input_data,
                       vector<Tensor> &output_data)
{
    input_tensor_.clear();
    for (int i = 0; i < input_num_; i++)
    {
        Tensor &item = const_cast<Tensor &>(input_data[i]);
        vector<int64_t> shape_data = item.GetShape().GetDataInt64();
        input_tensor_.emplace_back(
            Ort::Value::CreateTensor<float>(memory_info_,
                                            (float*)item.GetDataPtr(),
                                            item.Size(),
                                            shape_data.data(),
                                            shape_data.size()));
    }

    output_tensor_.clear();
    for (int i = 0; i < output_num_; i++)
    {
        Tensor &item = const_cast<Tensor &>(output_data[i]);
        vector<int64> shape_data = item.GetShape().GetDataInt64();
        output_tensor_.emplace_back(
            Ort::Value::CreateTensor<float>(memory_info_,
                                            (float*)item.GetDataPtr(),
                                            item.Size(),
                                            shape_data.data(),
                                            shape_data.size()));
    }
    try
    {
        session_->Run(Ort::RunOptions{nullptr},
                      input_names_.data(),
                      *input_tensor_.data(),
                      input_names_.size(),
                      output_names_.data(),
                      *output_tensor_.data(),
                      output_names_.size());
    }
    catch (const std::exception &ex)
    {
        printf("exception in session_->Run: %s\n", ex.what());
        return -1;
    }

    return 0;
}