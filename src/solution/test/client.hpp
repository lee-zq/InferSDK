#include "message.h"
#include "interface.h"

class Client{
public:
    int Send(message* msg){
        cv_server_->process(msg);
        return 0;
    }
    // 初始化msg
    int init_msg(){
        msg.id = 1;
        msg.data = 2;
        return 0;
    }

    virtual int init(void* cv_server_ptr){
        cv_server_ = cv_server_ptr;
        return 0;
    }
public:
    void* cv_server_ = nullptr;
    message msg;
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
