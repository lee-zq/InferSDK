#include "arch/instance.h"
#include "utils/base_func.hpp"
#include "module/classification/classifier.h"
#include "module/detection/detection.h"
#include "module/segmentation/seg.h"

int Instance::init(const std::string& init_param, const InferEngineParam& infer_param){
    std::vector<std::string> inst_init_param;
    int ret = split(init_param, inst_init_param, ",");
    if (ret != 0){
        std::cout << "split inst_init_param failed!" << std::endl;
        return -1;
    }
    if (inst_init_param.size() != 1){
        std::cout << "inst_init_param.size() != 1 current supported one module" << std::endl;
        return -1;
    }
    for (int i = 0; i < inst_init_param.size(); i++)
    {
        append_module(inst_init_param[i], infer_param);
    }

    std::string module_type = inst_init_param[0];
}

int Instance::compute(std::vector<cv::Mat>& input_imgs, void* results){
    return 0;
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
    IModule* module_inst = nullptr;
    if(task_type=="Classifier"){
        module_inst = new Classifier();
        int ret = module_inst->init(param);
        if (ret!=0){
          std::cout << "error" << std::endl; 
          return ret;  
        }
    }else if(task_type=="Detection"){
        module_inst = new Detection();
        int ret = module_inst->init(param);
        if (ret!=0){
          std::cout << "error" << std::endl; 
          return ret;  
        }
    }else {
        std::cout << "No supported task type : " << task_type << std::endl; 
    }
    module_map_.insert(std::make_pair(task_type, module_inst));
    return 0;
}


