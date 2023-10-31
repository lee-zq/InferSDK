#pragma once
#include "base_instance.h"
#include "base_module.h"
#include "cv_server/message.h"
#include "inst_pool.hpp"
#include <map>
#include <mutex>
#include <string>
#include <vector>


static std::mutex inst_mgr_mutex;

class InstManager
{
public:
    int init(const std::string& inst_mgr_cfg);
    int create_inst(TaskType task_type, int num = 1);
    int destroy_inst(TaskType task_type);
    int run(TaskType task_type, std::vector<cv::Mat>& input_imgs, void* result);
    int fini();
    ~InstManager()
    {
        delete inst_mgr_;
    }

    static InstManager* getInstManager()
    {
        std::lock_guard<std::mutex> lock(inst_mgr_mutex);
        if (!inst_mgr_)
        {
            inst_mgr_ = new InstManager();
        }
        return inst_mgr_;
    }

private:
    int parser_cfg(std::string cfg_path);
    InstManager(/* args */){};

private:
    bool initialized_ = false;
    static InstManager* inst_mgr_;
    std::map<TaskType, InstParamType> inst_param_map_;
    std::map<TaskType, InstPool*> task_inst_pool_;
    

};

#define InstMgr InstManager::getInstManager()