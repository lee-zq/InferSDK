#pragma once
#include "module/imodule.hpp"

#include <string>
#include <vector>
#include <mutex>

class InstManager
{
public:
    InstManager(/* args */);
    virtual int init(const InitParam& param);
    virtual int run(std::vector<cv::Mat>& input_imgs, std::vector<IResult*>& results) ;
    virtual int fini() ;
    virtual ~InstManager();

    int append_module(const std::string& module_type);
    InstManager* int getInstance(){
        std::lock_guard<std::mutex> lock(inst_mgr_mutex_);
        if (!infer_inst_){
            inst_mgr_ = new InstManager();
        }
        return inst_mgr_;
    }
private:
    InstManager::InstManager(/* args */){}

private:
    static std::mutex inst_mgr_mutex_;
    static Singleton* inst_mgr_;
    std::map<std::string, IModule*> module_map_;
};

#define InstMgr InstManager::getInstance()