#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <time.h>
#include <vector>

#include "com/utils/base_func.h"
#include "all_type.h"
#include "arch/module_factory.hpp"
#include "com/logger.h"
#include "cv_server/error_code.h"
#include "cv_server/message.h"
#include "detection.h"

static void letterbox_image(cv::Mat& img,
                            int w,
                            int h,
                            cv::Scalar out_bg_color,
                            cv::Mat& out)
{
    int new_w = img.cols;
    int new_h = img.rows;
    float resize_ratio = std::min((float)w / img.cols, (float)h / img.rows);
    new_w = (int)(img.cols * resize_ratio);
    new_h = (int)(img.rows * resize_ratio);
    cv::Mat resized_img;
    cv::resize(img,
               resized_img,
               cv::Size(new_w, new_h),
               0,
               0,
               cv::INTER_LINEAR);
    out = cv::Mat(h, w, CV_8UC3, out_bg_color);
    int dx = (w - new_w) / 2;
    int dy = (h - new_h) / 2;
    resized_img.copyTo(
        out(cv::Rect(dx, dy, resized_img.cols, resized_img.rows)));
}

int Detection::preproc(std::vector<cv::Mat>& input_imgs)
{
    if (input_imgs.size() != 1 || input_imgs[0].empty())
    {
        LError("input image is empty");
        return ERR_INVALID_VALUE;
    }

    int cur_in_b = input_imgs.size();
    int cur_in_c = input_imgs[0].channels();
    int cur_in_h = input_imgs[0].rows;
    int cur_in_w = input_imgs[0].cols;
    log_error_return(cur_in_b != 1 || cur_in_c != 3 || cur_in_h < 32 ||
                         cur_in_w < 32,
                     "input image shape is invalid");
    cv::Mat resized_img;
    letterbox_image(input_imgs[0],
                    model_input_w_,
                    model_input_h_,
                    cv::Scalar(128, 128, 128),
                    resized_img);
    resized_img.convertTo(resized_img, CV_32FC3);
    resized_img = resized_img / 255.f;
    std::vector<cv::Mat> channels;
    cv::split(resized_img, channels);

    input_datas_[0].Reshape(
        vector<int>{cur_in_b, cur_in_c, model_input_w_, model_input_w_}, Float32);
    float* input_datas_data_ptr0 = (float*)input_datas_[0].GetDataPtr();
    for (int c = 0; c < cur_in_c; c++)
    {
        float* src_ptr = (float*)channels[c].ptr<uchar>(0);
        float* dst_ptr = input_datas_data_ptr0 + c * model_input_w_ * model_input_h_;
        memcpy(dst_ptr, src_ptr, model_input_w_ * model_input_h_ * sizeof(float));
    }

    input_datas_[1].Reshape(vector<int>{1, 2}, Float32);
    float* input_datas_data_ptr1 = (float*)input_datas_[1].GetDataPtr();
    input_datas_data_ptr1[0] = input_imgs[0].cols;
    input_datas_data_ptr1[1] = input_imgs[0].rows;

    return 0;
}

int Detection::postproc(void* results)
{
    Tensor& out_boxes = output_datas_[0];
    Tensor& out_scores = output_datas_[1];
    Tensor& out_classes = output_datas_[2];
    std::vector<Bbox> detect_result;

    std::vector<int32_t> idx_info = out_classes.dump_to_vector<int32_t>();
    for (int n = 0; n < idx_info.size(); n=n+3)
    {
        int& b_idx = idx_info[n];
        int& c_idx = idx_info[n + 1];
        int& s_idx = idx_info[n + 2];
        Bbox bbox;
        bbox.class_id = idx_info[c_idx];
        bbox.score = out_scores[b_idx][c_idx][s_idx].item<float>();
        vector<float> ori_bbox = out_boxes[b_idx][s_idx].dump_to_vector<float>();
        bbox.x1 = ori_bbox[0];
        bbox.y1 = ori_bbox[1];
        bbox.x2 = ori_bbox[2];
        bbox.y2 = ori_bbox[3];
        detect_result.push_back(bbox);
    }
    std::string* out_str = static_cast<std::string*>(results);
    Bbox2Json(detect_result, *out_str);
    return 0;
}

int Detection::inference(std::vector<cv::Mat>& input_imgs, void* results)
{
    if (!is_init_)
    {
        std::cout << "Detection is not init!" << endl;
        return -1;
    }
    int ret = preproc(input_imgs);
    if (ret != 0)
    {
        std::cout << "Detection preproc failed!" << endl;
        return -1;
    }
    ret = infer_inst_->forward(input_datas_, output_datas_);
    if (ret != 0)
    {
        std::cout << "Detection forward failed!" << endl;
        return -1;
    }
    ret = postproc(results);
    if (ret != 0)
    {
        std::cout << "Detection postproc failed!" << endl;
        return -1;
    }
    return 0;
}

REGISTER_MODULE_CLASS(Detection)