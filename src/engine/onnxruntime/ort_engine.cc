#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include "onnxruntime_cxx_api.h"
#include "ort_engine.h"

using namespace std;

int ORTEngine::init(const InferEngineParam& param){
  param_ = param;
  memory_info_ = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

  session_option.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
  session_option.SetExecutionMode(ExecutionMode::ORT_PARALLEL);
  session_option.SetIntraOpNumThreads(param_.thread_num);
  if (param_.dev_type==CUDA)
  {
    OrtSessionOptionsAppendExecutionProvider_CUDA(session_option, param_.thread_num);
  }
  session_ = new Ort::Session(env, param_.onnx_path.c_str(), session_option);
  if (session_ == nullptr){
    spdlog::error("Ort::AllocatorWithDefaultOptions allocator is nullptr");
    return -1;
  }

  Ort::AllocatorWithDefaultOptions* allocator_ = new Ort::AllocatorWithDefaultOptions();
  if (allocator_ == nullptr){
    spdlog::error("Ort::AllocatorWithDefaultOptions allocator is nullptr");
    return -1;
  }

  input_num_ = session_->GetInputCount();
  output_num_ = session_->GetOutputCount();
  for (int i=0;i<input_num_;i++){
    input_names_.push_back(session_->GetInputName(i, *allocator_));
  }
  
  for (int i=0;i<output_num_;i++){
    output_names_.push_back(session_->GetOutputName(i, *allocator_));
  }

  input_tensor_.clear();
  output_tensor_.clear();
  return 0;
}

int ORTEngine::forward(const vector<Tensor>& input_data, vector<Tensor>& output_data){
  int ret = bind_io_data(input_data, output_data);
  ret = session->Run(Ort::RunOptions{nullptr}, input_names_.data(), input_tensor_.data(), input_names_.size(), 
              output_names_.data(), output_tensor_.data(), output_names_.size());
  return ret;
}

int ORTEngine::bind_io_data(const vector<Tensor>& input_data, vector<Tensor>& output_data){
    input_tensor_.clear();
    for (int i=0; i<input_num_; i++){
      input_tensor_.emplace_back(Ort::Value::CreateTensor<float>(memory_info_, 
                                                                input_data[i].GetDataPtr(), input_data[i].Size(), 
                                                                input_shape[i].Shape().GetDataPtr(), input_shape[i].Size()));
    }

    output_tensor_.clear();
    for (int i=0; i<output_num_; i++){
      output_tensor_.emplace_back(Ort::Value::CreateTensor<float>(memory_info_, 
                                                                  output_data[i].data(), output_data[i].size(), 
                                                                  output_shape[i].data(), output_shape[i].size()));
    }
}
