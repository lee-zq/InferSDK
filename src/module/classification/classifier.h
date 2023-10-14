#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>

#include "opencv2/opencv.hpp"

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
    virtual int init(const InferEngineParam& param) override;
    virtual int inference(std::vector<cv::Mat>& input_imgs, void* results) override;
    virtual int uninit() override;

  virtual int init(const InferEngineParam& param) {
    infer_inst_ = new ORTEngine();
    infer_inst_->init(param);
    
    input_shapes_ = infer_inst_->get_input_shapes();
    input_datas_.resize(infer_inst_->get_input_num());
    for (int i=0; i<input_datas_.size(); i++){
      input_datas_[i].Reshape(input_shapes_[i]);
    }
    output_shapes_ = infer_inst_->get_output_shapes();
    output_datas_.resize(infer_inst_->get_output_num());
    for (int i=0; i<output_datas_.size(); i++){
      output_datas_[i].Reshape(output_shapes_[i]);
    }
    is_init_ = true;
    return 0;
  };

private:
    int preproc(std::vector<cv::Mat>& input_img);
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