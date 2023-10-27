#pragma once

#include "cv_server/message.h"
#include "base_instance.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

typedef std::vector<std::pair<std::string, InferEngineParam>> InstParamType;

class CommonInst : public InstanceBase
{
public:
    virtual int init(const std::vector<std::pair<std::string, InferEngineParam>>& param) override;
    virtual int compute(std::vector<cv::Mat>& input_imgs, void* results) override;
    virtual int append_module(const std::string& task_type, const InferEngineParam& param) ;
    virtual int fini() override;
    virtual ~CommonInst(){};

private:
    // TaskType task_type_;
    // std::vector<std::pair<std::string, ModuleBase*>> modules_;
};