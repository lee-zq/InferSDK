#pragma once
#include "infer_face.h"

#include <string>
#include <vector>

class InferManager
{
public:
    InferManager(/* args */);
    virtual int init(const InitParam& param);
    virtual int run(std::vector<cv::Mat>& input_imgs, std::vector<IResult*>& results) ;
    virtual int finish() ;
    virtual ~InferManager();

    int create_inferinst(std::string task_type);

private:
    InferFace* infer_inst_;
};