#pragma once
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"  //文件输入流
#include "rapidjson/filewritestream.h" //文件输出流
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h" //文件格式化输出

#include <fstream>
#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include "dataset_class_id_map.hpp"

int render_img_with_bboxes(rapidjson::Value& bboxes, cv::Mat& img)
{
    for (rapidjson::SizeType i = 0; i < bboxes.Size(); i++)
    {
        rapidjson::Value& bbox = bboxes[i];
        int x1 = bbox["x1"].GetInt();
        int y1 = bbox["y1"].GetInt();
        int x2 = bbox["x2"].GetInt();
        int y2 = bbox["y2"].GetInt();
        int class_id = bbox["class_id"].GetInt();
        float score = bbox["score"].GetFloat();
        std::string textline = get_coco_class_name(class_id) + " " + std::to_string(score).substr(0, 4);
        cv::rectangle(img, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2);
        cv::putText(img, textline, cv::Point(x1, y1), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 1);
    }
    return 0;
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
    if (!doc.HasMember("bbox"))
    {
        std::cout << "json has no member image" << std::endl;
        return -1;
    }
    rapidjson::Value& bboxes = doc["bbox"];
    render_img_with_bboxes(bboxes, img);
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