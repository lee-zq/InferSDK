#include <algorithm>
#include <cmath>
#include <fstream>
#include <time.h>
#include <vector>

#include "arch/module_factory.hpp"
#include "cv_server/error_code.h"
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
    if (input_imgs[0].empty())
    {
        LError("input image is invalid");
        return ERR_INVALID_VALUE;
    }
    if (input_imgs[0].channels()!=3)
    {
        LError("input image channels is not 3");
        return ERR_INVALID_VALUE;
    }
    int input_channel = input_shapes_[0][1];
    int input_height = input_shapes_[0][2];
    int input_width = input_shapes_[0][3];
    cv::Mat resized_img;
    letterbox_image(input_imgs[0],
                    input_width,
                    input_height,
                    cv::Scalar(128, 128, 128),
                    resized_img);
    resized_img.convertTo(resized_img, CV_32FC3);
    resized_img = resized_img / 255.f;
    std::vector<cv::Mat> channels;
    cv::split(resized_img, channels);
    float* input_datas_data_ptr = (float*)input_datas_[0].GetDataPtr();

    for (int c = 0; c < input_channel; c++)
    {
        float* src_ptr = (float*)channels[c].ptr<uchar>(0);
        float* dst_ptr = input_datas_data_ptr + c * input_height * input_width;
        memcpy(dst_ptr, src_ptr, input_height * input_width * sizeof(float));
    }

    float* input_datas_data_ptr1 = (float*)input_datas_[1].GetDataPtr();
    input_datas_data_ptr1[0] = input_imgs[0].rows;
    input_datas_data_ptr1[1] = input_imgs[0].cols;

    return 0;
}

int Detection::postproc(void* results)
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