#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include "cv_inst.h"

using namespace std;

int CVInst::init(InitParam& param){
  memory_info_ = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

  session_option.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
  session_option.SetExecutionMode(ExecutionMode::ORT_PARALLEL);
  if (param.dev_type==cuda)
  {
    OrtSessionOptionsAppendExecutionProvider_CUDA(session_option, param.dev_type);
  }
  session = Ort::Session(env, param.onnx_path, session_option);

  OrtAllocator* allocator_ = new OrtAllocator();

  input_num_ = session.GetInputCount();
  output_num_ = session.GetOutputCount();
  for (int i=0;i<input_num_;i++){
    input_names_.push_back(session.GetInputName(i, allocator_));
  }
  
  for (int i=0;i<output_num_;i++){
    output_names_.push_back(session.GetOutputName(i, allocator_));
  }

  int ret = init_();
}

int CVInst::infer(vector<cv::Mat>& input_imgs, vector<IResult*>& results) {
  int ret = preproc(input_imgs);
  ret = compute();
  ret = postproc(results);
  return 0;
}

int CVInst::compute(){
  int ret = feed_io_data();
  session.Run(Ort::RunOptions{nullptr}, input_names_.data(), input_tensor_.data(), input_names_.size(), 
              output_names_.data(), output_tensor_.data(), output_names_.size());
}

int CVInst::feed_io_data(){
    vector<vector<float>>& input_data = GetInputData();
    vector<vector<int64_t>>& input_shape = GetInputShape();
    for (int i=0; i<input_num_; i++){
      input_tensor_[i] = Ort::Value::CreateTensor<float>(memory_info_, input_data[i].data(), input_data[i].size(), input_shape[i].data(), input_shape[i].size());
    }

    vector<vector<float>>& output_data = GetOutputData();
    vector<vector<int64_t>>& output_shape = GetOutputShape();
    for (int i=0; i<output_num_; i++){
      output_tensor_[i] = Ort::Value::CreateTensor<float>(memory_info_, output_data[i].data(), output_data[i].size(), output_shape[i].data(), output_shape[i].size());
    }
}
