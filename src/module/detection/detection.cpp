#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <time.h>
#include <vector>

#include "all_type.h"
#include "arch/module_factory.hpp"
#include "base_type.h"
#include "com/logger.h"
#include "com/utils/base_func.h"
#include "cv_server/error_code.h"
#include "cv_server/message.h"
#include "detection.h"

static void letterbox_image(const cv::Mat& img, int w, int h, cv::Scalar out_bg_color, cv::Mat& out)
{
    int new_w = img.cols;
    int new_h = img.rows;
    float resize_ratio = std::min((float)w / img.cols, (float)h / img.rows);
    new_w = (int)(img.cols * resize_ratio);
    new_h = (int)(img.rows * resize_ratio);
    cv::Mat resized_img;
    cv::resize(img, resized_img, cv::Size(new_w, new_h), 0, 0, cv::INTER_CUBIC);
    out = cv::Mat(h, w, CV_8UC3, out_bg_color);
    int dx = (w - new_w) / 2;
    int dy = (h - new_h) / 2;
    resized_img.copyTo(out(cv::Rect(dx, dy, resized_img.cols, resized_img.rows)));
}

int Detection::init(const ModuleParamType& param)
{
    infer_inst_ = new ORTEngine();
    infer_inst_->init(param.res_path, param.dev_type, param.dev_id, param.thread_num);

    input_shapes_ = infer_inst_->get_input_shapes();
    input_datas_.resize(input_shapes_.size());
    output_shapes_ = infer_inst_->get_output_shapes();
    output_datas_.resize(output_shapes_.size());

    // input_shapes_[0] = vector<int>{1, 3, 416, 416};
    // input_shapes_[1] = vector<int>{1, 2};
    // input_datas_.resize(infer_inst_->get_input_num());
    // for (int i = 0; i < input_datas_.size(); i++)
    // {
    //     input_datas_[i].Reshape(input_shapes_[i]);
    // }
    // output_datas_.resize(infer_inst_->get_output_num());
    // for (int i = 0; i < output_datas_.size(); i++)
    // {
    //     output_datas_[i].Reshape(output_shapes_[i]);
    // }
    is_init_ = true;
    return 0;
};

int Detection::preproc(const cv::Mat& input_img)
{
    if (input_img.empty())
    {
        LError("input image is empty");
        return ERR_INVALID_VALUE;
    }

    int cur_in_b = 1;
    int cur_in_c = input_img.channels();
    int cur_in_h = input_img.rows;
    int cur_in_w = input_img.cols;
    log_error_return(cur_in_b != 1 || cur_in_c != 3 || cur_in_h < 32 || cur_in_w < 32,
                     "input image shape is invalid",
                     ERR_INVALID_VALUE);
    cv::Mat resized_img;
    letterbox_image(input_img, model_input_w_, model_input_h_, cv::Scalar(128, 128, 128), resized_img);
    resized_img.convertTo(resized_img, CV_32FC3);
    resized_img = resized_img / 255.f;
    std::vector<cv::Mat> channels;
    cv::split(resized_img, channels);

    input_datas_[0].Reshape(vector<int>{cur_in_b, cur_in_c, model_input_w_, model_input_w_}, Float32);
    float* input_datas_data_ptr0 = (float*)input_datas_[0].GetDataPtr();
    for (int c = 0; c < cur_in_c; c++)
    {
        float* src_ptr = (float*)channels[c].ptr<uchar>(0);
        float* dst_ptr = input_datas_data_ptr0 + c * model_input_w_ * model_input_h_;
        memcpy(dst_ptr, src_ptr, model_input_w_ * model_input_h_ * sizeof(float));
    }

    input_datas_[1].Reshape(vector<int>{1, 2}, Float32);
    float* input_datas_data_ptr1 = (float*)input_datas_[1].GetDataPtr();
    input_datas_data_ptr1[0] = input_img.rows;
    input_datas_data_ptr1[1] = input_img.cols;

    return 0;
}

int Detection::postproc(void* result)
{
    Tensor& out_boxes = output_datas_[0];
    Tensor& out_scores = output_datas_[1];
    Tensor& out_classes = output_datas_[2];
    auto detect_result = static_cast<std::vector<DetectInfo>*>(result);
    detect_result->clear();

    std::vector<int32_t> idx_info = out_classes.dump_to_vector<int32_t>();
    for (int n = 0; n < idx_info.size(); n = n + 3)
    {
        int& b_idx = idx_info[n];
        int& c_idx = idx_info[n + 1];
        int& s_idx = idx_info[n + 2];

        vector<float> ori_bbox = out_boxes[b_idx][s_idx].dump_to_vector<float>();
        DetectInfo detect_info;
        detect_info.id = c_idx;
        detect_info.score = out_scores[b_idx][c_idx][s_idx].item<float>();
        detect_info.bbox.y1 = ori_bbox[0];
        detect_info.bbox.x1 = ori_bbox[1];
        detect_info.bbox.y2 = ori_bbox[2];
        detect_info.bbox.x2 = ori_bbox[3];
        detect_result->push_back(detect_info);
    }
    return 0;
}

int Detection::inference(const cv::Mat& input_img, void* result)
{
    log_error_return(is_init_ != true, "Detection is not init!", ERR_UNINITIALIZED);
    int ret = preproc(input_img);
    log_error_return(ret != ERR_SUCCESS, "Detection preproc failed!", ret);
    ret = infer_inst_->forward(input_datas_, output_datas_);
    log_error_return(ret != ERR_SUCCESS, "Detection forward failed!", ret);
    ret = postproc(result);
    log_error_return(ret != ERR_SUCCESS, "Detection postproc failed!", ret);
    return 0;
}

REGISTER_MODULE_CLASS(Detection)