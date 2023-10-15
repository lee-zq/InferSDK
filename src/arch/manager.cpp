#include "arch/manager.h"
#include <string>
#include <vector>
#include "classification/classifier.h"
#include "CVServer/message.h"

int InstManager::init(const std::string& manager_param){
    std::cout << "InstManager init" << std::endl;
    return 0;
}

int InstManager::create_inst(const int& type, const std::string& param, Instance** inst_ptr){
    Instance* inst = new Instance();
    InferEngineParam infer_param;
    inst->init(param, infer_param);
    *inst_ptr = inst;
    return 0;
}

int InstManager::destroy_inst(Instance* inst_ptr){
    delete inst_ptr;
    return 0;
}

int InstManager::run(Instance* inst_ptr, std::vector<cv::Mat>& input_imgs, void* result){
    inst_ptr->compute(input_imgs, result);
    return 0;
}

int InstManager::fini(){
    return 0;
}
