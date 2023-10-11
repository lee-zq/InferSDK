#pragma once
#include "module/imodule.hpp"

#include <string>
#include <vector>
#include <mutex>

class InstManager
{
public:
    virtual int init(const std::string& param);
    virtual int run(std::vector<cv::Mat>& input_imgs, std::vector<IResult*>& results) ;
    virtual int fini() ;
    virtual ~InstManager();

    int append_module(const std::string& module_type);
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
    std::map<std::string, IModule*> module_map_;
};

#define InstMgr InstManager::getInstance()