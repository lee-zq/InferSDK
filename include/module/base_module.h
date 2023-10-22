#pragma once
#include "all_type.h"
#include "com/logger.h"
#include "com/tensor.h"
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

class BaseModule
{
public:
    BaseModule(){}; // Module默认构造函数

    virtual int init(const InferEngineParam& param) = 0;
    virtual int inference(std::vector<cv::Mat>& input_imgs, void* results) = 0;
    virtual int uninit() = 0;
    virtual ~BaseModule(){};

    virtual std::vector<Tensor>& GetInputData()
    {
        return input_datas_;
    }
    virtual std::vector<Shape>& GetInputShape()
    {
        return input_shapes_;
    }
    virtual std::vector<Tensor>& GetOutputData()
    {
        return output_datas_;
    }
    virtual std::vector<Shape>& GetOutputShape()
    {
        return output_shapes_;
    }

protected:
    bool is_init_ = false;
    std::vector<Tensor> input_datas_;
    std::vector<Tensor> output_datas_;
    std::vector<Shape> input_shapes_;
    std::vector<Shape> output_shapes_;
};
