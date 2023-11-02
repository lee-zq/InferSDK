#pragma once
#include "com/utils//base_func.h"
#include "com/utils/conf_reader.h"
#include "cv_server/cv_server.h"
#include "cv_server/message.h"
#include "visual_func.hpp"
#include <algorithm>
#include <fstream>
#include <ios>
#include <string>

using namespace iSDK;

class Client
{
public:
    int init(std::string cfg_path = "../cfg/client.cfg")
    {

        parser_cfg(cfg_path);
        msg_.input = &in_data_;
        msg_.output = &out_data_;
        return 0;
    }
    int parser_cfg(const std::string& cfg_path)
    {
        cfg_path_ = cfg_path;
        ConfReader conf(cfg_path);
        conf.setSection("Client");
        supported_task_type_ = conf.readIntArray("request_task_type", ",");

        int thread_num = conf.readInt("thread_num", 1);
        std::string data_lst = conf.readStr("data_lst", "");
        save_result_ = conf.readBool("save_result", false);
        save_dir_ = conf.readStr("save_dir", "./save");
        visualize_result_ = conf.readBool("visualize_result", false);
        visualize_dir_ = conf.readStr("visualize_dir", "./visualize");
        return 0;
    }
    int start(void* cv_server)
    {
        cv_server_ = static_cast<CVServer*>(cv_server);
        for (auto task_type : supported_task_type_)
        {
            int ret = cv_server_->create_inst(static_cast<TaskType>(task_type), 1);
            if (ret != 0)
            {
                std::cout << "cv_server create_inst error" << std::endl;
                return -1;
            }
        }
        return 0;
    }
    int stop()
    {
        for (auto task_type : supported_task_type_)
        {
            int ret = cv_server_->destroy_inst(static_cast<TaskType>(task_type));
            if (ret != 0)
            {
                std::cout << "cv_server create_inst error" << std::endl;
                return -1;
            }
        }
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

    int process(std::string input_data_path, std::string visual_dir = "./")
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
                LWarn("parser data_list split error. process: {}, ret={}", img_paths[i], ret);
                continue;
            }
            img_path = item[0];
            task_type = std::atoi(item[1].c_str());
            id = std::atoi(item[2].c_str());
            cv::Mat input_img = cv::imread(img_path);
            ret = process(input_img, task_type, id);
            if (ret != 0)
            {
                LError("Client process error. ret={}", ret);
                continue;
            }
            // 打印输出信息
            std::cout << img_path << " output_info: " << out_data_.output_info << std::endl;
            // 保存输出信息
            if (save_result_)
            {
                std::string save_path = save_dir_ + "/" + getFileNameNoExt(img_path) + ".json";
                std::ofstream f(save_path, std::ios::out);
                f << out_data_.output_info;
            }
            // 可视化输出信息
            if (visualize_result_)
            {
                std::string save_path = visualize_dir_ + "/" + getFileName(img_path);
                render_img_with_json_str(out_data_.output_info, input_img);
                cv::imwrite(save_path, input_img);
            }
        }
        return 0;
    }

public:
    std::vector<int> supported_task_type_;
    CVServer* cv_server_ = nullptr;
    message msg_;
    InData in_data_;
    OutData out_data_;

    std::string cfg_path_;

    bool save_result_ = false;
    std::string save_dir_;
    bool visualize_result_ = false;
    std::string visualize_dir_;
};
