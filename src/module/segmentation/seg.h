#include <algorithm>
#include <cmath>
#include <fstream>
#include <time.h>

#include "all_type.h"
#include "opencv2/opencv.hpp"

#include "base_module.h"
#include "com/tensor.h"
#include "engine/infer_engine.h"
#include "engine/onnxruntime/ort_engine.h"

class Segmentation : public ModuleBase
{
public:
    Segmentation(){};
    Segmentation(const ModuleParamType& module_param)
    {
        module_param_ = module_param;
    };
    virtual int inference(const cv::Mat& input_img, void* result) override;
    virtual int uninit() override
    {
        if (infer_inst_ != nullptr)
        {
            infer_inst_->uninit();
            delete infer_inst_;
            infer_inst_ = nullptr;
        }
        return 0;
    }
    virtual int init(const ModuleParamType& param) override;

private:
    int preproc(const cv::Mat& input_img);
    int postproc(void* results);

private:
    const int class_num = 10;
    const int input_height = 32;
    const int input_width = 32;
    const int input_channel = 3;

    std::vector<float> mean_{0.4914, 0.4822, 0.4465};
    std::vector<float> std_{0.2023, 0.1994, 0.2010};

    InferEngine* infer_inst_ = nullptr;
};

// SPACE_END