#pragma once
#include <string>
#include <vector>
#include "../data_type.h"

class IModule
{
public:
    IModule(/* args */){};
    virtual int init(const InitParam& param) = 0;
    virtual int infer(std::vector<cv::Mat>& input_imgs, std::vector<IResult*>& results) = 0;
    virtual int fini() = 0;
    virtual ~IModule(){};
};


