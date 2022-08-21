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
  float* input_prt = input_[0].data();
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < input_height; i++) {
      for (int j = 0; j < input_width; j++) {
        float tmp = input_img[0].ptr<uchar>(i)[j * 3 + c];
        input_prt[c * input_height * input_width + i * input_width + j] = ((tmp) / 255.0 - mean_[c]) / std_[c];
      }
    }
  }
  return 0;
}


int Classifier::postproc(vector<IResult*>& results) {
  for (int n=0; n<input_.size();n++){
    for (int i=0;i<input_[0].size(); i++){
      ClassifyResult* result = static_cast<ClassifyResult*>(results[i]);
      auto& res = result->GetClassResult();
      res = (int)output_[n][i];
    }
  }
  return 0;
}