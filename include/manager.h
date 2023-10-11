#pragma once
#include "infer_face.h"

#include <string>
#include <vector>

class InstManager
{
public:
    InstManager(/* args */);
    virtual int init(const InitParam& param);
    virtual int run(std::vector<cv::Mat>& input_imgs, std::vector<IResult*>& results) ;
    virtual int fini() ;
    virtual ~InstManager();

    int create_inferinst(std::string task_type);

private:
    IModule* infer_inst_;
};