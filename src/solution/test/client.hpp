#include "cv_server/message.h"
#include "cv_server/cv_server.h"
#include "utils/base_func.hpp"

class Client{
public:
    int init(void* cv_server){
        cv_server_ = static_cast<CVServer*>(cv_server);
        msg_.input = &in_data_;
        msg_.output = &out_data_;
        return 0;
    }
    int send_msg(message* msg){
        int ret = cv_server_->process(msg);
        return ret;
    }
    int process(std::string img_path, int fid, int pid){
        msg_.pid = pid;
        msg_.fid = fid;
        in_data_.img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
        in_data_.img_info = img_path;
        in_data_.input_context = nullptr;

        out_data_.output_info = "";
        out_data_.output_context = nullptr;

        int ret = send_msg(&msg_);
        if(ret != 0){
            std::cout << "cv_server process error" << std::endl;
            return -1;
        }
        // 打印输出信息
        std::cout << "output_info: " << out_data_.output_info << std::endl;
        std::cout << "process success" << std::endl;
        return 0;
    }

    int process(std::string input_data_path){
        std::vector<std::string> img_paths = load_file(input_data_path);
        std::vector<std::string> item;
        std::string img_path;
        int fid;
        int pid;
        for(int i = 0; i < img_paths.size(); i++){
            int ret = split(img_paths[i], item, " ");
            if(ret != 0){
                LWarn("parser data_list split error. process: %s, ret=%d", img_paths[i], ret);
                continue;
            }
            img_path = item[0];
            fid = std::atoi(item[1].c_str());
            pid = std::atoi(item[2].c_str());
            ret = process(img_path, fid, pid);
            if(ret != 0){
                LError("Client commit error. ret=%d", ret);
                return -1;
            }
        }
        return 0;
    }
public:
    CVServer* cv_server_ = nullptr;
    message msg_;
    InData in_data_;
    OutData out_data_;
};

