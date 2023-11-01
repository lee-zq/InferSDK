#pragma once

#include "all_type.h"
#include "cv_server/message.h"
#include "base_module.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

class InstanceBase
{
public:
    virtual int init(const InstParamType& param) = 0;
    virtual int compute(cv::Mat* input_data, void* output_data) = 0;
    virtual int fini() = 0;
    virtual ~InstanceBase(){};

protected:
    TaskType task_type_;
    std::vector<std::pair<ModuleType, std::shared_ptr<ModuleBase>>> modules_;
};