#ifndef MESSAGE_H
#define MESSAGE_H
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <set>
#include <string>
#include <vector>

typedef struct _message
{
    int id;        // 进程号
    int task_type; // 模块号
    void* input;   // 输入数据 InData
    void* output;  // 输出数据 OutData
} message;

typedef struct _InData
{
    cv::Mat img;
    std::string img_info;
    void* input_context;
} InData;

typedef struct _OutData
{
    std::string output_info;
    void* output_context;
} OutData;

// task_type 类型枚举
enum TaskType
{
    NONE,
    CLASSIFY,
    DETECTION,
    SEG,
    LANE_DETECT,
    PERSON_DETECT,
    PERSON_POSE,
    SCENE_CLASSIFY,
    COUNT
};
#endif