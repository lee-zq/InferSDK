#ifndef MESSAGE_H
#define MESSAGE_H
#include "com/define.h"
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

SPACE_BEGIN

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

enum class TaskType
{
    INVALID_TYPE,
    COMMON,
    SCENE_ANALYSIS,
};

// task_type 类型枚举
enum class ModuleType
{
    InvalidType,
    Classification,
    Detection,
    Segmentation
};

SPACE_END

#endif