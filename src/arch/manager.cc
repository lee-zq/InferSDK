#include "module/manager.h"
#include "spdlog/spdlog.h"
#include <string>
#include <vector>
#include "classification/classifier.h"

int InstManager::init(const std::string& cfg){
    return 0;
}

int InstManager::run(const std::string& type ,cv::Mat& input_img, Result* result){
    auto module = module_map_.find(type);
    module->infer(input_img, result);
    return 0;
}

int InstManager::fini(){
    for (auto& module : module_map_){
        module->second->fini();
        delete module->second;
    }
    module_map_.clear();
    return 0;
}

InstManager::~InstManager(){

}

int InstManager::append_module(const std::string& task_type, CreateParam& param){
    IModule* module_inst_ = nullptr;
    if(task_type=="Classifier"){
        module_inst_ = new Classifier();
        int ret = module_inst->init(param);
        if (ret!=0){
          std::cout << "error" <<endl; 
          return ret;  
        }
    }else {
        std::cout << "No supported task type : " << task_type <<endl; 
    }
    module_map_.insert(std::make_pair(task_type, module_inst_));
    return 0;
}
