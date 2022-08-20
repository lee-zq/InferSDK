#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>

#include "cv_inst.h"

const int class_num = 10;
const int input_height = 32;
const int input_width = 32;
const int input_channel = 3;

const int batch_size = 1;

using namespace cv;
using namespace std;


class Classifier : public CVInst{
 public:
  Classifier(const char* onnx_path);


 private:
  virtual int feed_input(string& img_path) = 0;
  virtual int get_output(int& result) = 0;
  virtual int preproc() = 0;
  virtual int compute() = 0;
  virtual int postproc() = 0;

 private:

  std::array<float, batch_size * input_height * input_width * input_channel> input_;
  std::array<float, batch_size * class_num> output_;
  std::array<int64_t, 4> input_shape_{batch_size, input_channel, input_width, input_height};
  std::array<int64_t, 2> output_shape_{batch_size, class_num};

  Ort::Value input_tensor_{nullptr};
  Ort::Value output_tensor_{nullptr};
};