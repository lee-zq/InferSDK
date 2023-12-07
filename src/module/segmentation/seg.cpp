#include <algorithm>
#include <cmath>
#include <fstream>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/types_c.h>
#include <time.h>

#include "arch/module_factory.hpp"
#include "com/define.h"
#include "seg.h"
#include "tensor.h"
#include "utils/dataset_class_id_map.h"

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
    cv::Mat data_img;
    cv::cvtColor(input_img, data_img, CV_BGR2RGB);
    data_img.convertTo(data_img, CV_32FC3);
    data_img = data_img / 255.f;
    // std::vector<cv::Mat> channels;
    // cv::split(data_img, channels);

    // input_datas_[0].Reshape(vector<int>{1, data_img.channels(), data_img.rows, data_img.cols}, Float32);
    // float* input_datas_data_ptr0 = (float*)input_datas_[0].GetDataPtr();
    // for (int c = 0; c < channels.size(); c++)
    // {
    //     cv::Mat& channel = channels[c];
    //     channel = (channel - mean_[c]) / std_[c];
    //     float* src_ptr = (float*)channel.data;
    //     float* dst_ptr = input_datas_data_ptr0 + c * data_img.cols * data_img.rows;
    //     memcpy(dst_ptr, src_ptr, data_img.cols * data_img.rows * sizeof(float));
    // }
    input_datas_[0].FromImages(std::vector<cv::Mat>{data_img}, mean_, std_);
    // auto debug_data = input_datas_[0].dump_to_vector<float>();
    return 0;
}

int Segmentation::postproc(void* result)
{
    auto seg_result = (std::vector<SegInfo>*)result;
    seg_result->clear();
    Tensor& prob = output_datas_[0];
    auto shape_vec = prob.GetShape();
    cv::Mat prob_mat(shape_vec[2], shape_vec[3], CV_32FC1, prob.GetDataPtr());
    cv::Mat all_label_mat(shape_vec[2], shape_vec[3], CV_8UC1);
    all_label_mat.setTo(0);

    for (int c = 0; c < shape_vec[1]; c++)
    {
        Tensor channel = prob[0][c];
        for (int h = 0; h < shape_vec[2]; h++)
        {
            for (int w = 0; w < shape_vec[3]; w++)
            {
                float* prob = (float*)channel.GetDataPtr(h * shape_vec[3] + w);
                if (*prob > prob_mat.at<float>(h, w))
                {
                    prob_mat.at<float>(h, w) = *prob;
                    all_label_mat.at<uchar>(h, w) = c;
                }
            }
        }
    }
    // cv::imwrite("label.png", all_label_mat * 10);
    cv::Mat log_img = all_label_mat * 10;
    cv::cvtColor(log_img, log_img, CV_GRAY2BGR);
    int min_area_thres = shape_vec[2] * shape_vec[3] / 100;
    int StructuringElementSize = shape_vec[2] * shape_vec[3] / 10000;
    for (int class_id = 1; class_id < shape_vec[1]; class_id++)
    {
        cv::Mat cur_class_mat;
        cv::compare(all_label_mat, class_id, cur_class_mat, cv::CMP_EQ);
        // //统计图像中连通域的个数
        cv::Mat labels;
        int numComponents = cv::connectedComponents(cur_class_mat, labels);
        for (int i = 1; i < numComponents; ++i)
        {
            cv::Mat mask = (labels == i);
            // 闭合操作
            cv::morphologyEx(
                mask,
                mask,
                cv::MORPH_CLOSE,
                cv::getStructuringElement(cv::MORPH_RECT, cv::Size(StructuringElementSize, StructuringElementSize)));
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
            auto contour = contours[0];

            // 计算边界框面积
            cv::Rect boundingRect = cv::boundingRect(contour);
            int area = boundingRect.width * boundingRect.height;
            if (area < min_area_thres) // 面积小于200舍弃
                continue;

            // 计算轮廓凸包
            std::vector<cv::Point> hull;
            cv::convexHull(contour, hull);
            // 绘制边界框
            cv::rectangle(log_img, boundingRect, cv::Scalar(0, 255, 0), 2);
            // 绘制凸包
            cv::polylines(log_img, hull, true, cv::Scalar(0, 0, 255), 2);
            // 绘制轮廓
            cv::drawContours(log_img, contours, -1, cv::Scalar(255, 0, 0), 2);
            // 保存结果
            SegInfo seg_info;
            seg_info.id = class_id;
            seg_info.score = 1.f;
            seg_info.name = get_voc_class_name(class_id);
            seg_info.bbox.x1 = boundingRect.x;
            seg_info.bbox.y1 = boundingRect.y;
            seg_info.bbox.x2 = boundingRect.x + boundingRect.width;
            seg_info.bbox.y2 = boundingRect.y + boundingRect.height;
            for (auto point : hull)
            {
                seg_info.contour.push_back(Point{point.x,point.y,1.f});
            }
            seg_result->push_back(seg_info);
        }
    }
    cv::imwrite("./visual/seg_log_img.png", log_img);
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