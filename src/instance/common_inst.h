#pragma once

#include "cv_server/message.h"
#include "base_instance.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

class CommonInst : public InstanceBase
{
public:
    virtual int init(const InstParamType& param) override;
    virtual int compute(std::vector<cv::Mat>& input_imgs, void* results) override;
    virtual int fini() override;
    virtual ~CommonInst(){};

private:
    // TaskType task_type_;
    // std::vector<std::pair<std::string, ModuleBase*>> modules_;
};