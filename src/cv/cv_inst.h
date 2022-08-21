#include "onnxruntime_cxx_api.h"
#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include "opencv2/opencv.hpp"
#include "cv/infer_face.h"
#include "opencv2/opencv.hpp"

using namespace std;

class CVInst : public InferFace{
 public:
  CVInst(){};
  virtual int init(InitParam& param);
  virtual int infer(vector<cv::Mat>& input_imgs, vector<IResult*>& results);

  virtual int feed_io_data();
  virtual int compute();

  virtual int init_() = 0;

  virtual int preproc(std::vector<cv::Mat>& input_imgs) = 0;
  virtual int postproc(vector<IResult*>& results) = 0;

  virtual vector<vector<float>>& GetInputData() = 0;
  virtual vector<vector<int64_t>>& GetInputShape() = 0;
  virtual vector<vector<float>>& GetOutputData() = 0;
  virtual vector<vector<int64_t>>& GetOutputShape() = 0;

  virtual int finish(){};

 public:
  Ort::MemoryInfo memory_info_{nullptr};
  Ort::Allocator*  allocator_;

  int input_num_;
  int output_num_;
  vector<Ort::Value> input_tensor_;
  vector<Ort::Value> output_tensor_;
  vector<const char*> input_names_;
  vector<const char*> output_names_;
  device_type device_type_;
  
  Ort::SessionOptions session_option;
  Ort::Env env{ORT_LOGGING_LEVEL_WARNING, "cv_inst"};
  Ort::Session session{nullptr};
};