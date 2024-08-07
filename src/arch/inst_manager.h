#pragma once
#include "base_instance.h"
#include "base_module.h"
#include "com/define.h"
#include "cv_server/message.h"
#include "inst_pool.hpp"
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

SPACE_BEGIN

class InstManager
{
public:
    int init(const std::string& inst_mgr_cfg);
    int create_inst(TaskType task_type, int num = 1);
    int destroy_inst(TaskType task_type);
    int run(TaskType task_type, cv::Mat* input_data, void* output_data);
    int fini();

    static InstManager* getInstManager()
    {
        static std::mutex inst_mgr_mutex;
        if (inst_mgr_ == nullptr)
        {
            std::lock_guard<std::mutex> lock(inst_mgr_mutex);
            if (inst_mgr_ == nullptr)
            {
                inst_mgr_ = new InstManager();
            }
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

SPACE_END