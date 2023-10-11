#pragma once
#include <opencv2/imgproc.hpp>

typedef enum device_type{
    cpu,
    cuda,
    npu, 
    other
} device_type;

typedef struct InitParam
{
   const char* onnx_path;
   device_type dev_type;
} InitParam;


class IResult{
public:
    virtual int& GetClassResult() = 0;
    virtual float& GetScoreResult() = 0;
};

class ClassifyResult : public IResult{
public:
    int class_result;
    float score;
    virtual int& GetClassResult(){
        return class_result;
    }
    virtual float& GetScoreResult(){
        return score;
    }
};
