#include <iostream>
#include <fstream>
#include <string>
#include <vector>


static int load_img_path(std::string& file_path, std::vector<std::string>& img_lst, std::vector<int>& label_lst) {
  std::ifstream f(file_path.c_str());
  if (!f.is_open()) {
    std::cout << "open file failed" << std::endl;
    return -1;
  }
  std::string img_path;
  int label;
  while (std::getline(f, img_path)) {
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

static int save_result(std::string& file_path, std::vector<int>& results) {
  std::ofstream f(file_path.c_str());
  if (!f.is_open()) {
    std::cout << "open file failed" << std::endl;
    return -1;
  }
  for (auto& res : results) {
    f << res << std::endl;
  }
  f.close();
  return 0;
}

static float cal_acc(std::vector<int>& labels, std::vector<int>& results) {
  float TP = 0.;
  for (int i = 0; i < labels.size(); i++) {
    if (labels[i] == results[i]) {
      TP++;
    }
  }
  return TP / labels.size();
}
