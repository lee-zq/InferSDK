#include "module/manager.h"
#include "spdlog/spdlog.h"
#include <string>
#include <vector>
#include "../module/classification/classifier.h"

int InstManager::init(const InitParam& param){
    infer_inst_->init(param);
    return 0;
}

int InstManager::run(std::vector<cv::Mat>& input_imgs, std::vector<IResult*>& results){
    infer_inst_->infer(input_imgs, results);
    return 0;
}

int InstManager::fini(){
    if (infer_inst_)
        delete infer_inst_;
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
            
        }
    }else {
        std::cout << "No supported task type : " << task_type <<endl; 
    }

}
