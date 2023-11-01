#include <algorithm>
#include <cmath>
#include <fstream>
#include <time.h>

#include "base_module.h"
#include "com/tensor.h"
#include "cv_server/error_code.h"
#include "engine/infer_engine.h"
#include "engine/onnxruntime/ort_engine.h"
#include "opencv2/opencv.hpp"

class Classification : public ModuleBase
{
public:
    Classification(){};
    Classification(const ModuleParamType& module_param)
    {
        module_param_ = module_param;
    };
    virtual int inference(const cv::Mat& input_img, void* result) override;
    virtual int uninit() override;

    virtual int init(const ModuleParamType& param) override;

private:
    int preproc(const cv::Mat& input_img);
    int postproc(void* results);

private:
    std::vector<float> mean_{0.485, 0.456, 0.406};
    std::vector<float> std_{0.229, 0.224, 0.225};
    InferEngine* infer_inst_ = nullptr;
};

// SPACE_END