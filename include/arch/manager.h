#pragma once
#include "module/imodule.h"
#include "instance.h"
#include <string>
#include <vector>
#include <mutex>

class InstManager
{
public:
    virtual int init(const std::string& manager_param);
    virtual int create_inst(const FID& type, const std::string& param, Instance** inst_ptr);
    virtual int destroy_inst(Instance* inst_ptr);
    virtual int run(Instance* inst_ptr, std::vector<cv::Mat>& input_imgs, void* results) ;
    virtual int fini();
    virtual ~InstManager();

    int append_module(const FID& module_type);
    static InstManager* getInstance(){
        std::lock_guard<std::mutex> lock(inst_mgr_mutex_);
        if (!inst_mgr_){
            inst_mgr_ = new InstManager();
        }
        return inst_mgr_;
    }
private:
    InstManager(/* args */){}

private:
    static std::mutex inst_mgr_mutex_;
    static InstManager* inst_mgr_;
    
};

#define InstMgr InstManager::getInstance()