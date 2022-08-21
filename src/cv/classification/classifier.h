#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>

#include "../cv_inst.h"

using namespace cv;
using namespace std;


class Classifier : public CVInst{
 public:
  Classifier(){};
  virtual int preproc(std::vector<cv::Mat>& input_img);
  virtual int postproc(vector<IResult*>& results);

  virtual int init_() {};

  virtual vector<vector<float>>& GetInputData(){
    return input_;
  }
  virtual vector<vector<int64_t>>& GetInputShape(){
    return input_shape_;
  }
  virtual vector<vector<float>>& GetOutputData(){
    return output_;
  }
  virtual vector<vector<int64_t>>& GetOutputShape(){
    return output_shape_;
  }

 private:
  vector<std::vector<float>> input_;
  vector<std::vector<float>> output_;
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