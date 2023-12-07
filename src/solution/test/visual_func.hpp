#pragma once
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"  //文件输入流
#include "rapidjson/filewritestream.h" //文件输出流
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h" //文件格式化输出

#include "opencv2/opencv.hpp"
#include <fstream>
#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <string>
#include <vector>

// 典型颜色赤橙黄绿青蓝紫
static std::map<std::string, cv::Scalar> color_map = {
    {"red", cv::Scalar(0, 0, 255)},      // red
    {"orange", cv::Scalar(0, 128, 255)}, // orange
    {"yellow", cv::Scalar(0, 255, 255)}, // yellow
    {"green", cv::Scalar(0, 255, 0)},    // green
    {"cyan", cv::Scalar(255, 255, 0)},   // cyan
    {"blue", cv::Scalar(255, 0, 0)},     // blue
    {"purple", cv::Scalar(255, 0, 255)}  // purple
};

void render_img_with_class_result(rapidjson::Value& class_result, cv::Mat& img)
{
    int class_id = class_result["id"].GetInt();
    float score = class_result["score"].GetFloat();
    std::string name = class_result["name"].GetString();
    std::string textline = name + "|" + std::to_string(class_id) + "|" + std::to_string(score).substr(0,6);
    cv::putText(img, textline, cv::Point(0, img.rows), cv::FONT_HERSHEY_SIMPLEX, 0.8, color_map["red"], 1);
}

void render_img_with_detect_result(rapidjson::Value& detect_result, cv::Mat& img)
{
    for (rapidjson::SizeType i = 0; i < detect_result.Size(); i++)
    {
        rapidjson::Value& data = detect_result[i];
        int class_id = data["id"].GetInt();
        float score = data["score"].GetFloat();
        std::string name = data["name"].GetString();
        rapidjson::Value& bbox = data["bbox"];
        int x1 = bbox["x1"].GetInt();
        int y1 = bbox["y1"].GetInt();
        int x2 = bbox["x2"].GetInt();
        int y2 = bbox["y2"].GetInt();
        std::string textline = name + "" + std::to_string(class_id) + "|" + std::to_string(score).substr(0,6);
        cv::rectangle(img, cv::Point(x1, y1), cv::Point(x2, y2), color_map["green"], 2);
        cv::putText(img, textline, cv::Point(x1, y1), cv::FONT_HERSHEY_SIMPLEX, 0.8, color_map["red"], 1);
    }
}

void render_img_with_seg_result(rapidjson::Value& seg_result, cv::Mat& img)
{
    for (rapidjson::SizeType i = 0; i < seg_result.Size(); i++)
    {
        rapidjson::Value& data = seg_result[i];
        int class_id = data["id"].GetInt();
        float score = data["score"].GetFloat();
        std::string name = data["name"].GetString();
        rapidjson::Value& contour = data["contour"];
        std::string textline = name + "" + std::to_string(class_id) + "|" + std::to_string(score).substr(0,6);
        std::vector<cv::Point> points;
        for (rapidjson::SizeType j = 0; j < contour.Size(); j++)
        {
            rapidjson::Value& point = contour[j];
            points.emplace_back(point["x"].GetInt(), point["y"].GetInt());
        }
        for (int j = 0; j < points.size(); j++)
        {
            cv::line(img, points[j], points[(j + 1)%points.size()], color_map["green"], 2);
        }
        cv::putText(img, textline, points[0], cv::FONT_HERSHEY_SIMPLEX, 0.8, color_map["red"], 1);
    }
}

int render_img_with_json_str(std::string& json_string, cv::Mat& img)
{
    rapidjson::Document doc;
    doc.Parse(json_string.c_str());
    if (doc.HasParseError())
    {
        std::cout << "parse json error" << std::endl;
        return -1;
    }
    if (!doc.IsObject())
    {
        std::cout << "json is not object" << std::endl;
        return -1;
    }
    static std::vector<std::string> result_key_vec = {"class_info", "detect_info", "seg_info"};
    for (auto& key : result_key_vec)
    {
        if (doc.HasMember(key.c_str()))
        {
            rapidjson::Value& result = doc[key.c_str()];
            if (key == "class_info")
            {
                render_img_with_class_result(result, img);
            }
            else if (key == "detect_info")
            {
                render_img_with_detect_result(result, img);
            }
            else if (key == "seg_info")
            {
                render_img_with_seg_result(result, img);
            }
        }
    }
    std::cout << "visualize img finish." << std::endl;
    return 0;
}

std::string getFileName(std::string filepath)
{
    return filepath.substr(filepath.find_last_of('/') + 1);
}

std::string getFileNameNoExt(std::string filepath)
{
    std::string file_name = getFileName(filepath);
    std::string::size_type pos = file_name.rfind(".");
    return file_name.substr(0, pos);
}