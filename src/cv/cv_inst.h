#include "onnxruntime_cxx_api.h"
#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "infersdk/cv/infer_face.h"
#include "opencv2/opencv.hpp"

class CVInst : public InferFace{
 public:
  CVInst(const char* onnx_path, dev_t device_type);
  virtual int forward();
 private:
  virtual int feed_input(std::string& img_path) = 0;
  virtual int get_output(int& result) = 0;
  virtual int preproc() = 0;
  virtual int compute() = 0;
  virtual int postproc() = 0;

 private:

  Ort::Value input_tensor_{nullptr};
  Ort::Value output_tensor_{nullptr};
  dev_t device_type;
  Ort::MemoryInfo allocator_info_;
  Ort::SessionOptions session_option;
  Ort::Env env{ORT_LOGGING_LEVEL_WARNING, "cv_inst"};
  Ort::Session session{nullptr};
};