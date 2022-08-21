#include "cv/manager.h"

#include <string>
#include <vector>
#include "../cv/classification/classifier.h"

Manager::Manager(/* args */){
    
}
int Manager::init(){

}
int Manager::run(){

}
int Manager::finish(){

}
Manager::~Manager(){

}

int Manager::create_inferinst(std::string task_type, InferFace** pHandle){
    if(task_type=="classification"){
        *pHandle = new Classifier();
    }else if(task_type=="detection"){

    }else {
        std::cout << "No supported task type : " << task_type <<endl; 
    }
}
