#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>

#include "classifier.h"

int Classifier::preproc(std::vector<cv::Mat>& input_img) {
  int batchsize = input_img.size();
  if(batchsize!=batch_size){
    std::cout<<"暂时不考虑多batch"<<endl;
  }
  if (input_img[0].empty()){
    std::cout << "img is empty! | " << endl;
    return -1;
  }
  //Mat dst(input_height, input_width, CV_8UC3);
  //resize(img, dst, Size(row, col));
  //cvtColor(img, dst, COLOR_BGR2RGB);
  // float* ori_input_prt = input_img[0].data();
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < input_height; i++) {
      for (int j = 0; j < input_width; j++) {
        float tmp = input_img[0].ptr<uchar>(i)[j * 3 + c];
        input_data_[0][c * input_height * input_width + i * input_width + j] = ((tmp) / 255.0 - mean_[c]) / std_[c];
      }
    }
  }
  return 0;
}

int Classifier::postproc(vector<IResult*>& results) {
  for (int n=0; n<output_data_.size();n++){
    int class_result = std::distance(output_data_[n].begin(), std::max_element(output_data_[n].begin(), output_data_[n].end()));
    ClassifyResult* result = static_cast<ClassifyResult*>(results[n]);
    auto& res = result->GetClassResult();
    res = class_result;
  }
  return 0;
}

REGISTER_MODULE_CLASS("Classifier") 