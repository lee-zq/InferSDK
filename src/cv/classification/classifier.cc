#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>

#include "cv_inst.h"

CVInst::CVInst(const char* onnx_path) {
  auto allocator_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
  input_tensor_ = Ort::Value::CreateTensor<float>(allocator_info, input_.data(), input_.size(), input_shape_.data(), input_shape_.size());
  output_tensor_ = Ort::Value::CreateTensor<float>(allocator_info, output_.data(), output_.size(), output_shape_.data(), output_shape_.size());
  session_option.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
  session_option.SetExecutionMode(ExecutionMode::ORT_PARALLEL);
  OrtSessionOptionsAppendExecutionProvider_CUDA(session_option, 0);
  session = Ort::Session(env, onnx_path, session_option);
}

int CVInst::feed_input(string& img_path) {
  cv::Mat img = cv::imread(img_path);
  if (img.empty()){
    std::cout << "img is empty! | " << img_path << endl;
    return -1;
  }
  //Mat dst(input_height, input_width, CV_8UC3);
  //resize(img, dst, Size(row, col));
  //cvtColor(img, dst, COLOR_BGR2RGB);
  float* input_prt = input_.data();
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < input_height; i++) {
      for (int j = 0; j < input_width; j++) {
        float tmp = img.ptr<uchar>(i)[j * 3 + c];
        input_prt[c * input_height * input_width + i * input_width + j] = ((tmp) / 255.0 - mean_[c]) / std_[c];
      }
    }
  }
  return 0;
}

int CVInst::forward() {
  session.Run(Ort::RunOptions{nullptr}, input_names.data(), &input_tensor_, 1, output_names.data(), &output_tensor_, 1);
  return 0;
}

int CVInst::get_output(int& result) {
  result = std::distance(output_.begin(), std::max_element(output_.begin(), output_.end()));
  return 0;
}
