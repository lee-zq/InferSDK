#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>

#include "detection.h"
#include "arch/module_factory.hpp"

int Detection::preproc(std::vector<cv::Mat>& input_imgs) {
  if (input_imgs.size() != 1) {
    std::cout << "input_img.size() != 1 no supported | " << input_imgs.size() << " != 1"  << endl;
    return -1;
  }
  int batchsize = input_imgs.size();
  if(batchsize!=1){
    std::cout<<"暂时不考虑多batch"<<endl;
  }
  if (input_imgs[0].empty()){
    std::cout << "img is empty! | " << endl;
    return -1;
  }

  float* input_datas_data_ptr = (float*)input_datas_[0].GetDataPtr();
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < input_height; i++) {
      for (int j = 0; j < input_width; j++) {
        float tmp = input_imgs[0].ptr<uchar>(i)[j * 3 + c];
        input_datas_data_ptr[c * input_height * input_width + i * input_width + j] = ((tmp) / 255.0 - mean_[c]) / std_[c];
      }
    }
  }
  return 0;
}

int Detection::postproc(void* results) {
  // static_cast<int*>(results)[0] = 0;
  for (int n=0; n<output_datas_.size();n++){
    Tensor& item = output_datas_[n];
    // int class_result = std::distance(output_datas_[n].begin(), std::max_element(output_data_[n].begin(), output_data_[n].end()));
    // res = class_result;
  }
  return 0;
}

int Detection::inference(std::vector<cv::Mat>& input_imgs, void* results){
  if (!is_init_){
    std::cout << "Detection is not init!" << endl;
    return -1;
  }
  int ret = preproc(input_imgs);
  if (ret != 0){
    std::cout << "Detection preproc failed!" << endl;
    return -1;
  }
  ret = infer_inst_->forward(input_datas_, output_datas_);
  if (ret != 0){
    std::cout << "Detection forward failed!" << endl;
    return -1;
  }
  ret = postproc(results);
  if (ret != 0){
    std::cout << "Detection postproc failed!" << endl;
    return -1;
  }
  return 0;
}

REGISTER_MODULE_CLASS(Detection) 