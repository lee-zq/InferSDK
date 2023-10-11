#ifndef MESSAGE_H
#define MESSAGE_H
#include <vector>
#include <iostream>
#include <cstdio>
#include <string>
#include <map>
#include <set>
#include <mutex>
#include <memory>
#include <algorithm>
#include <opencv2/opencv.hpp>

typedef struct _message{
    int pid;        // 进程号
    int fid;        // 模块号
    void* input;    // 输入数据
    void* output;   // 输出数据
}message;

typedef struct InData{
    cv::Mat img;
    std::string img_info;
}InData;

typedef struct OutData{
    std::string output_info;
}OutData;

// fid 类型枚举
enum class FID{
    NONE,
    LANE_DETECT,
    PERSON_DETECT,
    PERSON_POSE,
    SCENE_CLASSIFY,
    COUNT
};
#endif