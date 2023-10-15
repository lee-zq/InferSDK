#pragma once

#include "module/imodule.h"

#include <string>
#include <vector>
#include <mutex>
#include <map>

class Instance{
public:
    virtual int init(const std::vector<std::pair<std::string,InferEngineParam>>& param);
    virtual int compute(std::vector<cv::Mat>& input_imgs, void* results);
    virtual int append_module(const std::string& task_type, const InferEngineParam& param);
    virtual int fini();
    virtual ~Instance(){};

private:
    std::map<std::string, IModule*> module_map_;
};