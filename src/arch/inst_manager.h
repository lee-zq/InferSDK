#pragma once
#include "arch/instance.h"
#include "cv_server/message.h"
#include "module/imodule.h"
#include <mutex>
#include <string>
#include <vector>

static std::mutex inst_mgr_mutex;

class InstManager
{
public:
    int init(const std::string& manager_param);
    int create_inst(TaskType, Instance** inst_ptr);
    int destroy_inst(Instance* inst_ptr);
    int run(Instance* inst_ptr,
            std::vector<cv::Mat>& input_imgs,
            void* results);
    int fini();
    ~InstManager()
    {
        delete inst_mgr_;
    }

    static InstManager* getInstance()
    {
        std::lock_guard<std::mutex> lock(inst_mgr_mutex);
        if (!inst_mgr_)
        {
            inst_mgr_ = new InstManager();
            // inst_mgr_->init("info");
        }
        return inst_mgr_;
    }

private:
    InstManager(/* args */)
    {
    }

private:
    static InstManager* inst_mgr_;
    std::map<TaskType, InstParamType> inst_param_map_;
};

#define InstMgr InstManager::getInstance()