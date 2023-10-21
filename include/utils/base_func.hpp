#include <iostream>
#include <fstream>
#include <string>
#include <vector>


inline std::vector<std::string> load_file(const std::string& file_path) {
  std::ifstream f(file_path.c_str());
  if (!f.is_open()) {
    std::cout << "open file failed" << std::endl;
    return {};
  }
  std::vector<std::string> data_lst;
  std::string item;
  while (std::getline(f, item)) {
    if (item.size() > 0) {
      data_lst.push_back(item);
    }
  }
  f.close();
  return data_lst;
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


// split string by delim
static int split(const std::string& str, std::vector<std::string>& ret_, std::string sep = ",") {
  if (str.empty()) {
    return 0;
  }
  ret_.clear();
  std::string tmp;
  std::string::size_type pos_begin = str.find_first_not_of(sep);
  std::string::size_type comma_pos = 0;
  while (pos_begin != std::string::npos) {
    comma_pos = str.find(sep, pos_begin);
    if (comma_pos != std::string::npos) {
      tmp = str.substr(pos_begin, comma_pos - pos_begin);
      pos_begin = comma_pos + sep.length();
    } else {
      tmp = str.substr(pos_begin);
      pos_begin = comma_pos;
    }
    if (!tmp.empty()) {
      ret_.push_back(tmp);
      tmp.clear();
    }
  }
  return 0;
}