#include "arch/instance.h"
#include "utils/base_func.hpp"
#include "module/classification/classifier.h"
#include "module/detection/detection.h"
#include "module/segmentation/seg.h"

int Instance::init(const std::vector<std::pair<std::string,InferEngineParam>>& param){

    if (param.size() != 1){
        std::cout << "inst_init_param.size() != 1 current supported one module" << std::endl;
        return -1;
    }
    for (int i = 0; i < param.size(); i++)
    {
        append_module(param[i].first, param[i].second);
    }
    return 0;
}

int Instance::compute(std::vector<cv::Mat>& input_imgs, void* results){
    for (auto& module : module_map_){
        module.second->inference(input_imgs, results);
    }
}

int Instance::fini(){
    for (auto& module : module_map_){
        module.second->uninit();
        delete module.second;
    }
    module_map_.clear();
    return 0;
}

int Instance::append_module(const std::string& task_type, const InferEngineParam& param){
    IModule* module = nullptr;
    if(task_type=="Classify"){
        module = new Classifier();
        int ret = module->init(param);
        if (ret!=0){
          std::cout << "error" << std::endl; 
          return ret;  
        }
    }else if(task_type=="Detection"){
        module = new Detection();
        int ret = module->init(param);
        if (ret!=0){
          std::cout << "error" << std::endl; 
          return ret;  
        }
    }else {
        std::cout << "No supported task type : " << task_type << std::endl; 
    }
    module_map_.insert(std::make_pair(task_type, module));
    return 0;
}


