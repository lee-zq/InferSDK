#pragma once
#include <algorithm>
#include <cmath>
#include <fstream>
#include <time.h>
#include <vector>

#include "com/tensor.h"
#include "onnxruntime_cxx_api.h"
#include "opencv2/opencv.hpp"

#include "engine/infer_engine.h"

using namespace std;

class ORTEngine : public InferEngine
{
public:
    ORTEngine(){};
    ~ORTEngine(){};
    virtual int init(const InferEngineParam& param) override;
    virtual int forward(const vector<Tensor>& input_data,
                        vector<Tensor>& output_data) override;
    virtual int async_forward(const vector<Tensor>& input_data,
                              vector<Tensor>& output_data) override
    {
        return 0;
    }
    virtual int uninit()
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
    InferEngineParam param_;
    Ort::MemoryInfo memory_info_{nullptr};
    Ort::AllocatorWithDefaultOptions* allocator_{nullptr};
    vector<Ort::Value> input_tensor_;
    vector<Ort::Value> output_tensor_;

private:
    vector<const char*> ort_input_names_;
    vector<const char*> ort_output_names_;
    Ort::SessionOptions session_option;
    Ort::Env env{ORT_LOGGING_LEVEL_WARNING, "onnxruntime"};
    Ort::Session* session_{nullptr};
};