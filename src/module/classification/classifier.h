#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>

#include "../cv_module.h"

// SPACE_BEGIN

using namespace cv;
using namespace std;

class Classifier : public CVModule{
 public:
  Classifier(){};
  virtual int preproc(std::vector<cv::Mat>& input_img);
  virtual int postproc(vector<IResult*>& results);

  virtual int init_() {
    input_shape_.emplace_back(vector<int64_t>{batch_size, input_channel, input_height, input_width});
    output_shape_.emplace_back(vector<int64_t>{batch_size, class_num});
    input_data_.resize(batch_size);
    input_data_[0].resize(batch_size*input_channel*input_height*input_width);
    output_data_.resize(batch_size);
    output_data_[0].resize(batch_size*class_num);

    return 0;
  };

  virtual vector<vector<float>>& GetInputData(){
    return input_data_;
  }
  virtual vector<vector<int64_t>>& GetInputShape(){
    return input_shape_;
  }
  virtual vector<vector<float>>& GetOutputData(){
    return output_data_;
  }
  virtual vector<vector<int64_t>>& GetOutputShape(){
    return output_shape_;
  }

 private:
  vector<std::vector<float>> input_data_;
  vector<std::vector<float>> output_data_;
  vector<std::vector<int64_t>> input_shape_;
  vector<std::vector<int64_t>> output_shape_;

  const int class_num = 10;
  const int input_height = 32;
  const int input_width = 32;
  const int input_channel = 3;

  const int batch_size = 1;
  std::vector<float> mean_{0.4914, 0.4822, 0.4465};
  std::vector<float> std_{0.2023, 0.1994, 0.2010};

  ClassifyResult result_;
};

// SPACE_END