#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "cv/manager.h"

using namespace cv;
using namespace std;

class ClassifierAgent {
 public:
  ClassifierAgent(const char* onnx_path, device_type cuda) {
    InitParam param{onnx_path, cuda};
    manager_ = new InferManager();
    manager_->create_inferinst(std::string("classification"));
    manager_->init(param);
  }

  int forward(vector<cv::Mat>& input_imgs) {
    vector<IResult*> outputs;
    results_.resize(input_imgs.size());
    for (auto& ele : results_){
      outputs.push_back(&ele);
    }
    manager_->run(input_imgs, outputs);
    return 0;
  }

  vector<ClassifyResult> get_results(){
    return results_;
  }

  ~ClassifierAgent(){
    if (manager_){
      manager_->finish();      
    }
  }

 private:
  InferManager* manager_=nullptr;
  vector<ClassifyResult> results_;
};

static int load_img_path(string& file_path, vector<string>& img_lst, vector<int>& label_lst) {
  ifstream f(file_path.c_str());
  if (!f.is_open()) {
    cout << "open file failed" << endl;
    return -1;
  }
  string img_path;
  int label;
  while (getline(f, img_path)) {
    if (img_path.size() > 0) {
      img_lst.push_back(img_path);
      auto iter = img_path.find(".");
      label = std::atoi(img_path.substr(--iter, iter).c_str());
      label_lst.push_back(label);
    }
  }
  f.close();
  return 0;
}

static int save_result(string& file_path, vector<int>& results) {
  ofstream f(file_path.c_str());
  if (!f.is_open()) {
    cout << "open file failed" << endl;
    return -1;
  }
  for (auto& res : results) {
    f << res << endl;
  }
  f.close();
  return 0;
}

static float cal_acc(vector<int>& labels, vector<int>& results) {
  float TP = 0.;
  for (int i = 0; i < labels.size(); i++) {
    if (labels[i] == results[i]) {
      TP++;
    }
  }
  return TP / labels.size();
}

int main(int argc, char* argv[]) {
  if (argc>3){
    cerr << "argc count error!!!"<< endl;
    return -1;
  }
  const char* onnx_path = argv[1];
  string img_path_file = argv[2];
  vector<string> img_lst;
  vector<int> label_lst;
  vector<int> results;
  load_img_path(img_path_file, img_lst, label_lst);
  
  int result;
  ClassifierAgent classifier(onnx_path, device_type::cuda);

  int img_num = img_lst.size();
  clock_t start = clock();
  for (int i = 0; i < img_num; i++) {
    std::vector<cv::Mat> batch_imgs;
    result = -1;
    cv::Mat img = cv::imread(img_lst[i]);
    if (img.empty()){
      std::cout << "img is empty | " << img_lst[i] << endl;
    }
    batch_imgs.push_back(img);
    
    int ret = classifier.forward(batch_imgs);
    if (ret){
      continue;
    }
    vector<ClassifyResult> result = classifier.get_results();
    results.push_back(result[0].class_result);

    printf("\r %.2f %%", (float)i / img_num * 100);
  }
  float time_cost = (clock() - start)/CLOCKS_PER_SEC * 1000;

  float acc = cal_acc(label_lst, results);
  std::cout << "Total Time cost: " << time_cost << " ms" << std::endl;
  std::cout << "Average Time cost: " << time_cost / img_lst.size() << " ms" << std::endl;
  std::cout << "Test Acc:  " << acc << std::endl;

  // system("pause");
  return 0;
}
