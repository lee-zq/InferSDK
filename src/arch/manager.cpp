#include "arch/manager.h"
#include "spdlog/spdlog.h"
#include <string>
#include <vector>
#include "classification/classifier.h"
#include "CVServer/message.h"

int InstManager::init(const std::string& cfg){
    return 0;
}

int InstManager::create_inst(const FID& type, const std::string& param, Instance** inst_ptr){
    Instance* inst = new Instance();
    inst->init(param);
    *inst_ptr = inst;
    return 0;
}

int InstManager::destroy_inst(Instance* inst_ptr){
    delete inst_ptr;
    return 0;
}

int InstManager::run(Instance* inst_ptr, const std::string& type ,cv::Mat& input_img, void* result){
    inst_ptr->compute(type, input_img, result);
}

int InstManager::fini(){
    return 0;
}

InstManager::~InstManager(){

}
