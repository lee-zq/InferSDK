#include <algorithm>
#include <cmath>
#include <fstream>
#include <time.h>

#include "arch/module_factory.hpp"
#include "com/define.h"
#include "seg.h"

SPACE_BEGIN

int Segmentation::init(const ModuleParamType& param)
{
    infer_inst_ = new ORTEngine();
    infer_inst_->init(param.res_path, param.dev_type, param.dev_id, param.thread_num);

    input_shapes_ = infer_inst_->get_input_shapes();
    input_datas_.resize(infer_inst_->get_input_num());
    // for (int i = 0; i < input_datas_.size(); i++)
    // {
    //     input_datas_[i].Reshape(input_shapes_[i]);
    // }
    output_shapes_ = infer_inst_->get_output_shapes();
    output_datas_.resize(infer_inst_->get_output_num());
    // for (int i = 0; i < output_datas_.size(); i++)
    // {
    //     output_datas_[i].Reshape(output_shapes_[i]);
    // }
    is_init_ = true;
    return 0;
};

int Segmentation::preproc(const cv::Mat& input_img)
{
    if (input_img.empty())
    {
        std::cout << "img is empty! | " << endl;
        return -1;
    }

    float* input_datas_data_ptr = (float*)input_datas_[0].GetDataPtr();
    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < input_height; i++)
        {
            for (int j = 0; j < input_width; j++)
            {
                float tmp = input_img.ptr<uchar>(i)[j * 3 + c];
                input_datas_data_ptr[c * input_height * input_width + i * input_width + j] =
                    ((tmp) / 255.0 - mean_[c]) / std_[c];
            }
        }
    }
    return 0;
}

int Segmentation::postproc(void* result)
{
    // static_cast<int*>(results)[0] = 0;
    for (int n = 0; n < output_datas_.size(); n++)
    {
        Tensor& item = output_datas_[n];
        // int class_result = std::distance(output_datas_[n].begin(), std::max_element(output_data_[n].begin(), output_data_[n].end()));
        // res = class_result;
    }
    return 0;
}

int Segmentation::inference(const cv::Mat& input_img, void* result)
{
    if (!is_init_)
    {
        std::cout << "Segmentation is not init!" << endl;
        return -1;
    }
    int ret = preproc(input_img);
    if (ret != 0)
    {
        std::cout << "Segmentation preproc failed!" << endl;
        return -1;
    }
    ret = infer_inst_->forward(input_datas_, output_datas_);
    if (ret != 0)
    {
        std::cout << "Segmentation forward failed!" << endl;
        return -1;
    }
    ret = postproc(result);
    if (ret != 0)
    {
        std::cout << "Segmentation postproc failed!" << endl;
        return -1;
    }
    return 0;
}

REGISTER_MODULE_CLASS(Segmentation)

SPACE_END