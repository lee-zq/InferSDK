#pragma once
#include "all_type.h"
#include "com/define.h"
#include "com/tensor.h"
#include "com/utils/dataset_class_id_map.h"
#include "com/logger.h"
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

SPACE_BEGIN

class ModuleBase
{
public:
    ModuleBase(){}; // Module默认构造函数

    virtual int init(const ModuleParamType& param) = 0;
    virtual int inference(const cv::Mat& input_img, void* result) = 0;
    virtual int uninit() = 0;
    virtual ~ModuleBase(){};

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

    ModuleParamType module_param_;
};

SPACE_END