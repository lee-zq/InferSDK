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
    void* input;    // 输入数据 InData
    void* output;   // 输出数据 OutData
}message;

typedef struct _InData{
    cv::Mat img;
    std::string img_info;
    void* input_context;
}InData;

typedef struct _OutData{
    std::string output_info;
    void* output_context;
}OutData;

// fid 类型枚举
enum FID{
    NONE,
    CLASSIFY,
    LANE_DETECT,
    PERSON_DETECT,
    PERSON_POSE,
    SCENE_CLASSIFY,
    COUNT
};
#endif