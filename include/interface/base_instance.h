#pragma once

#include "cv_server/message.h"
#include "base_module.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

typedef std::vector<std::pair<std::string, InferEngineParam>> InstParamType;

class InstanceBase
{
public:
    virtual int init(const InstParamType& param) = 0;
    virtual int compute(std::vector<cv::Mat>& input_imgs, void* results) = 0;
    virtual int fini() = 0;
    virtual ~InstanceBase(){};

protected:
    TaskType task_type_;
    std::vector<std::pair<std::string, ModuleBase*>> modules_;
};