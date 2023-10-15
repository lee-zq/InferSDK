#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "classifier.h"
#include "arch/module_factory.hpp"
#include "CVServer/message.h"

int Classifier::preproc(std::vector<cv::Mat>& input_imgs) {
  if (input_imgs.size() != 1) {
    std::cout << "input_img.size() != 1 no supported | " << input_imgs.size() << " != 1"  << endl;
    return -1;
  }
  if (input_imgs[0].empty()){
    std::cout << "img is empty! | " << endl;
    return -1;
  }
  auto model_input_shape = input_shapes_[0].GetData();
  int input_batchsize = model_input_shape[1];
  int input_channel = model_input_shape[1];
  int input_height = model_input_shape[2];
  int input_width = model_input_shape[3];
  cv::Mat resized_img;
  cv::resize(input_imgs[0], resized_img, cv::Size(input_width, input_height));
  if (resized_img.cols != input_width || resized_img.rows != input_height || resized_img.channels() != input_channel){
    std::cout << "input_img size not match! | " << resized_img.cols << "x" << resized_img.rows << "x" << resized_img.channels() << " != " << input_width << "x" << input_height << "x" << input_channel << endl;
    return -1;
  }

  float* input_datas_data_ptr = (float*)input_datas_[0].GetDataPtr();
  for (int c = 0; c < 1; c++) {
    for (int i = 0; i < input_height; i++) {
      for (int j = 0; j < input_width; j++) {
        float tmp = resized_img.ptr<uchar>(i)[j * 3 + c];
        input_datas_data_ptr[c * input_height * input_width + i * input_width + j] = tmp;//((tmp) / 255.0 - mean_[c]) / std_[c];
      }
    }
  }
  return 0;
}

int Classifier::postproc(void* results) {
  // static_cast<int*>(results)[0] = 0;
  for (int n=0; n<output_datas_.size();n++){
    Tensor& item = output_datas_[n];
    vector<float> output_data;
    output_data.assign((float*)item.GetDataPtr(), (float*)item.GetDataPtr() + item.Size());
    int class_result = std::distance(output_data.begin(), std::max_element(output_data.begin(), output_data.end()));
    OutData* out_data = static_cast<OutData*>(results);
    out_data->output_info = std::to_string(class_result);
  }
  return 0;
}

int Classifier::inference(std::vector<cv::Mat>& input_imgs, void* results){
  if (!is_init_){
    std::cout << "Classifier is not init!" << endl;
    return -1;
  }
  int ret = preproc(input_imgs);
  if (ret != 0){
    std::cout << "Classifier preproc failed!" << endl;
    return -1;
  }
  ret = infer_inst_->forward(input_datas_, output_datas_);
  if (ret != 0){
    std::cout << "Classifier forward failed!" << endl;
    return -1;
  }
  ret = postproc(results);
  if (ret != 0){
    std::cout << "Classifier postproc failed!" << endl;
    return -1;
  }
  return 0;
}

REGISTER_MODULE_CLASS(Classifier) 