#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>

#include "opencv2/opencv.hpp"
#include "cv_server/error_code.h"
#include "module/imodule.h"
#include "common/tensor.h"
#include "engine/infer_engine.h"
#include "engine/onnxruntime/ort_engine.h"

class Classifier : public IModule{
public:
    Classifier(){};
    Classifier(const ModuleParam& module_param){
        // 此处添加Module构造函数传入的结构体参数, 解析并保存到成员变量中，在后续其他函数调用时使用
    };
    virtual int inference(std::vector<cv::Mat>& input_imgs, void* results) override;
    virtual int uninit() override;

    virtual int init(const InferEngineParam& param) override ;

private:
    int preproc(std::vector<cv::Mat>& input_img);
    int postproc(void* results);

private:
    std::vector<float> mean_{0.485, 0.456, 0.406};
    std::vector<float> std_{0.229, 0.224, 0.225};
    InferEngine* infer_inst_ = nullptr;
};

// SPACE_END