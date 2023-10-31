#include "classifier.h"
#include "arch/module_factory.hpp"
#include "com/logger.h"
#include "com/utils//base_func.h"
#include "cv_server/message.h"
#include "opencv2/opencv.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <opencv2/core/types.hpp>
#include <time.h>

int Classifier::init(const ModuleParamType& param)
{
    infer_inst_ = new ORTEngine();
    infer_inst_->init(param.res_path, param.dev_type, param.dev_id, param.thread_num);

    input_shapes_ = infer_inst_->get_input_shapes();
    for (auto& shape : input_shapes_)
    {
        if (shape[0] == -1)
        {
            shape[0] = 1;
            LWarn("model' input shape batch size is -1 (dynamic), set to 1");
        }
    }
    input_datas_.resize(infer_inst_->get_input_num());

    for (int i = 0; i < input_datas_.size(); i++)
    {
        input_datas_[i].Reshape(input_shapes_[i]);
    }
    output_shapes_ = infer_inst_->get_output_shapes();
    for (auto& shape : output_shapes_)
    {
        if (shape[0] == -1)
        {
            shape[0] = 1;
            LWarn("model' output shape batch size is -1 (dynamic), set to 1");
        }
    }
    output_datas_.resize(infer_inst_->get_output_num());
    for (int i = 0; i < output_datas_.size(); i++)
    {
        output_datas_[i].Reshape(output_shapes_[i]);
    }
    is_init_ = true;
    return 0;
}

int Classifier::uninit()
{
    if (infer_inst_ != nullptr)
    {
        infer_inst_->uninit();
        delete infer_inst_;
        infer_inst_ = nullptr;
    }
    return 0;
}

int Classifier::preproc(std::vector<cv::Mat>& input_imgs)
{
    if (input_imgs[0].empty())
    {
        LError("input image is empty!");
        return ERR_INVALID_VALUE;
    }

    int input_channel = input_shapes_[0][1];
    int input_height = input_shapes_[0][2];
    int input_width = input_shapes_[0][3];
    cv::Mat resized_img;
    cv::resize(input_imgs[0], resized_img, cv::Size(input_width, input_height));
    if (resized_img.channels() != input_channel)
    {
        return ERR_INVALID_VALUE;
    }
    resized_img.convertTo(resized_img, CV_32FC3);
    resized_img = resized_img / cv::Scalar(255, 255, 255);
    resized_img = (resized_img - cv::Scalar(mean_[0], mean_[1], mean_[2])) / cv::Scalar(std_[0], std_[1], std_[2]);
    std::vector<cv::Mat> channels;
    cv::split(resized_img, channels);
    float* input_datas_data_ptr = (float*)input_datas_[0].GetDataPtr();

    for (int c = 0; c < input_channel; c++)
    {
        float* src_ptr = (float*)channels[c].ptr<float>(0);
        float* dst_ptr = input_datas_data_ptr + c * input_height * input_width;
        memcpy(dst_ptr, src_ptr, input_height * input_width * sizeof(float));
    }
    input_datas_[0].dump_to_file("input.bin");
    return 0;
}

int Classifier::postproc(void* results)
{
    for (int n = 0; n < output_datas_.size(); n++)
    {
        Tensor& item = output_datas_[n];
        vector<float> output_data;
        output_data.assign((float*)item.GetDataPtr(), (float*)item.GetDataPtr() + item.Size());
        softmax(output_data);
        int class_result = std::distance(output_data.begin(), std::max_element(output_data.begin(), output_data.end()));
        OutData* out_data = static_cast<OutData*>(results);
        out_data->output_info = "id:" + std::to_string(class_result) + " score:" + std::to_string(output_data[class_result]);
    }
    return 0;
}

int Classifier::inference(std::vector<cv::Mat>& input_imgs, void* results)
{
    if (!is_init_)
    {
        LError("Module Classifier is not initialized!");
        return ERR_UNINITIALIZED;
    }
    int ret = preproc(input_imgs);
    log_error_return(ret, "Module Classifier preproc failed!", ret);
    ret = infer_inst_->forward(input_datas_, output_datas_);
    log_error_return(ret, "Module Classifier forward failed!", ret);
    ret = postproc(results);
    log_error_return(ret, "Module Classifier postproc failed!", ret);
    return ERR_SUCCESS;
}

REGISTER_MODULE_CLASS(Classifier)