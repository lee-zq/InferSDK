#include "onnxruntime_cxx_api.h"
#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include "opencv2/opencv.hpp"

#include "cv/infer_face.h"

using namespace cv;
using namespace std;

class ClassifierAgent {
 public:
  ClassifierAgent(const char* onnx_path, device_type cuda) {
    auto allocator_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    input_tensor_ = Ort::Value::CreateTensor<float>(allocator_info, input_.data(), input_.size(), input_shape_.data(), input_shape_.size());
    output_tensor_ = Ort::Value::CreateTensor<float>(allocator_info, output_.data(), output_.size(), output_shape_.data(), output_shape_.size());
    session_option.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    session_option.SetExecutionMode(ExecutionMode::ORT_PARALLEL);
    OrtSessionOptionsAppendExecutionProvider_CUDA(session_option, 0);
    session = Ort::Session(env, onnx_path, session_option);
  }

  int set_input(string& img_path) {
    cv::Mat img = cv::imread(img_path);
    if (img.empty()){
      std::cout << "img is empty! | " << img_path << endl;
      return -1;
    }
    //Mat dst(input_height, input_width, CV_8UC3);
    //resize(img, dst, Size(row, col));
    //cvtColor(img, dst, COLOR_BGR2RGB);
    float* input_prt = input_.data();
    for (int c = 0; c < 3; c++) {
      for (int i = 0; i < input_height; i++) {
        for (int j = 0; j < input_width; j++) {
          float tmp = img.ptr<uchar>(i)[j * 3 + c];
          input_prt[c * input_height * input_width + i * input_width + j] = ((tmp) / 255.0 - mean_[c]) / std_[c];
        }
      }
    }
    return 0;
  }

  int forward() {
    session.Run(Ort::RunOptions{nullptr}, input_names.data(), &input_tensor_, 1, output_names.data(), &output_tensor_, 1);
    return 0;
  }

  int get_result(int& result) {
    result = std::distance(output_.begin(), std::max_element(output_.begin(), output_.end()));
    return 0;
  }

 private:
  vector<const char*> input_names{"img"};
  vector<const char*> output_names{"output"};
  std::array<float, batch_size * input_height * input_width * input_channel> input_;
  std::array<float, batch_size * class_num> output_;
  std::array<int64_t, 4> input_shape_{batch_size, input_channel, input_width, input_height};
  std::array<int64_t, 2> output_shape_{batch_size, class_num};

  Ort::Value input_tensor_{nullptr};
  Ort::Value output_tensor_{nullptr};

  Ort::SessionOptions session_option;
  Ort::Env env{ORT_LOGGING_LEVEL_WARNING, "test"};

  Ort::Session session{nullptr};

  std::vector<float> mean_{0.4914, 0.4822, 0.4465};
  std::vector<float> std_{0.2023, 0.1994, 0.2010};
};

int load_img_path(string& file_path, vector<string>& img_lst, vector<int>& label_lst) {
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

int save_result(string& file_path, vector<int>& results) {
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

float cal_acc(vector<int>& labels, vector<int>& results) {
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
  Classifier classifier(onnx_path);

  int img_num = img_lst.size();
  clock_t start = clock();
  for (int i = 0; i < img_num; i++) {
    result = -1;
    cv::Mat img = cv::imread(img_lst[i]);
    if (img.empty()){
      std::cout << "img is emp! | " << img_lst[i] << endl;
    }
    
    int ret = classifier.set_input(img_lst[i]);
    if (ret){
      continue;
    }
    classifier.forward();
    classifier.get_result(result);
    results.push_back(result);

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
