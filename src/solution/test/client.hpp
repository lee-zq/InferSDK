#include "CVServer/message.h"
#include "../CVServer/cv_server.h"

class Client{
public:
    int init(void* cv_server){
        cv_server_ = static_cast<CVServer*>(cv_server);
        msg_.input = &in_data_;
        msg_.output = &out_data_;
        return 0;
    }
    int Send(std::string img_path, int fid, int pid){
        msg_.pid = pid;
        msg_.fid = fid;
        in_data_.img = cv::imread(img_path);
        in_data_.img_info = img_path;
        in_data_.input_context = nullptr;

        out_data_.output_info = "";
        out_data_.output_context = nullptr;

        int ret = cv_server_->process(&msg_);
        if(ret != 0){
            std::cout << "cv_server process error" << std::endl;
            return -1;
        }
        // 打印输出信息
        std::cout << "output_info: " << out_data_.output_info << std::endl;
        std::cout << "process success" << std::endl;
        return 0;
    }

public:
    CVServer* cv_server_ = nullptr;
    message msg_;
    InData in_data_;
    OutData out_data_;
};

