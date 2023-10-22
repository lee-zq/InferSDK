#pragma once
#include "com/utils//base_func.h"
#include "cv_server/cv_server.h"
#include "cv_server/message.h"
#include <string>

class Client
{
public:
    int init(void* cv_server)
    {
        cv_server_ = static_cast<CVServer*>(cv_server);
        msg_.input = &in_data_;
        msg_.output = &out_data_;
        return 0;
    }
    int send_msg(message* msg)
    {
        int ret = cv_server_->process(msg);
        return ret;
    }
    int process(const cv::Mat& img,
                int task_type,
                int id,
                std::string img_info = "",
                void* input_context = nullptr,
                void* output_context = nullptr)
    {
        msg_.id = id;
        msg_.task_type = task_type;
        in_data_.img = img;
        in_data_.img_info = img_info;
        in_data_.input_context = input_context;

        out_data_.output_info = "";
        out_data_.output_context = output_context;

        int ret = send_msg(&msg_);
        if (ret != 0)
        {
            std::cout << "cv_server process error" << std::endl;
            return -1;
        }
        return 0;
    }

    int process(std::string input_data_path)
    {
        std::vector<std::string> img_paths = load_file(input_data_path);
        std::vector<std::string> item;
        std::string img_path;
        int task_type;
        int id;
        for (int i = 0; i < img_paths.size(); i++)
        {
            int ret = split(img_paths[i], item, " ");
            if (ret != 0)
            {
                LWarn("parser data_list split error. process: %s, ret=%d",
                      img_paths[i],
                      ret);
                continue;
            }
            img_path = item[0];
            task_type = std::atoi(item[1].c_str());
            id = std::atoi(item[2].c_str());
            cv::Mat input_img = cv::imread(img_path);
            ret = process(input_img, task_type, id);
            if (ret != 0)
            {
                LError("Client process error. ret=%d", ret);
                return -1;
            }
            // 打印输出信息
            std::cout << img_path << " output_info: " << out_data_.output_info
                      << std::endl;
        }
        return 0;
    }

public:
    CVServer* cv_server_ = nullptr;
    message msg_;
    InData in_data_;
    OutData out_data_;
};
