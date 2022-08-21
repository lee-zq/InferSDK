#pragma once
#include <opencv2/imgproc.hpp>

typedef enum device_type{
    cpu,
    cuda,
    other
} device_type;

typedef struct InitParam
{
   const char* onnx_path;
   device_type dev_type;
} InitParam;


class IResult{
public:
    virtual int& GetClassResult() = 0;
    virtual float& GetScoreResult() = 0;
};

class ClassifyResult : public IResult{
public:
    int class_result;
    float score;
    virtual int& GetClassResult(){
        return class_result;
    }
    virtual float& GetScoreResult(){
        return score;
    }
};

// class DetectResult : public IResult{
// public:
//     vector<int> class_results;
//     vector<array<float, 4>> bboxes;
//     vector<float> scores;
// };


// class SegResult : public IResult{
// public:
//     vector<int> class_results;
//     vector<array<float, 4>> bboxes;
//     vector<float> scores;
//     // vector<cv::>> counters;
// };