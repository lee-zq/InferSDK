#pragma once
#include <algorithm>
#include <cmath>
#include <fstream>
#include <time.h>
#include <vector>

#include "com/define.h"
#include "com/tensor.h"
#include "onnxruntime_cxx_api.h"
#include "opencv2/opencv.hpp"

#include "engine/infer_engine.h"

SPACE_BEGIN

using namespace std;

class ORTEngine : public InferEngine
{
public:
    ORTEngine(){};
    ~ORTEngine(){};
    virtual int init(const std::string& res_path, DeviceType dev_type, int dev_id, int thread_num) override;
    virtual int forward(const vector<Tensor>& input_data, vector<Tensor>& output_data) override;
    virtual int async_forward(const vector<Tensor>& input_data, vector<Tensor>& output_data) override
    {
        return 0;
    }
    Ort::Value CreateOrtValueFromTensor(Tensor& tensor);
    int CopyOrtValue2Tensor(Ort::Value& value, Tensor& tensor);

    virtual int uninit() override
    {
        if (session_)
        {
            delete session_;
        }
        if (allocator_)
        {
            delete allocator_;
        }
        return 0;
    };

public:
    Ort::MemoryInfo memory_info_{nullptr};
    Ort::AllocatorWithDefaultOptions* allocator_{nullptr};
    vector<Ort::Value> input_tensor_;
    vector<Ort::Value> output_tensor_;

private:
    vector<const char*> input_names_;
    vector<const char*> output_names_;
    Ort::SessionOptions session_option;
    Ort::Env env{ORT_LOGGING_LEVEL_WARNING, "onnxruntime"};
    Ort::Session* session_{nullptr};
};

SPACE_END