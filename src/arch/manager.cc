#include "cv/manager.h"

#include <string>
#include <vector>
#include "../cv/classification/classifier.h"

InferManager::InferManager(/* args */){
    
}
int InferManager::init(const InitParam& param){
    infer_inst_->init(param);
    return 0;
}

int InferManager::run(std::vector<cv::Mat>& input_imgs, std::vector<IResult*>& results){
    infer_inst_->infer(input_imgs, results);
    return 0;
}

int InferManager::finish(){
    if (infer_inst_)
        delete infer_inst_;
    return 0;
}

InferManager::~InferManager(){

}

int InferManager::create_inferinst(std::string task_type){
    if(task_type=="classification"){
        infer_inst_ = new Classifier();
    }else if(task_type=="detection"){

    }else {
        std::cout << "No supported task type : " << task_type <<endl; 
    }
}
