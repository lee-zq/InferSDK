#include "cv/manager.h"

#include <string>
#include <vector>
#include "../cv/classification/classifier.h"

InstManager::InstManager(/* args */){
    
}
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

int InstManager::create_inferinst(std::string task_type){
    if(task_type=="classification"){
        infer_inst_ = new Classifier();
    }else if(task_type=="detection"){

    }else {
        std::cout << "No supported task type : " << task_type <<endl; 
    }
}
