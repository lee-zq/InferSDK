#pragma once
#include "infer_face.h"

#include <string>
#include <vector>

class Manager
{
public:
    Manager(/* args */);
    virtual int init();
    virtual int run() ;
    virtual int finish() ;
    virtual ~Manager();

private:
    int create_inferinst(std::string task_type, InferFace** handle);

private:
    InferFace* inferInst_;
};