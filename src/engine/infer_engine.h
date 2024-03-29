#pragma once
#include "com/define.h"
#include "com/tensor.h"
#include <map>
#include <string>
#include <vector>
// 推理引擎接口类

SPACE_BEGIN

using namespace std;

class InferEngine
{
public:
    InferEngine(){};
    virtual int init(const std::string& res_path, DeviceType dev_type, int dev_id, int thread_num) = 0;
    virtual int forward(const vector<Tensor>& input_data, vector<Tensor>& output_data) = 0;
    virtual int async_forward(const vector<Tensor>& input_data, vector<Tensor>& output_data) = 0;
    virtual int uninit() = 0;

    virtual const vector<string>& get_input_names()
    {
        return input_names_;
    }
    virtual const vector<Shape>& get_input_shapes()
    {
        return input_shapes_;
    }
    virtual int get_input_num()
    {
        return input_num_;
    }
    virtual const vector<string>& get_output_names()
    {
        return output_names_;
    }
    virtual const vector<Shape>& get_output_shapes()
    {
        return output_shapes_;
    }
    virtual int get_output_num()
    {
        return output_num_;
    }

    virtual ~InferEngine(){};

protected:
    int input_num_;
    int output_num_;
    vector<string> input_names_;
    vector<string> output_names_;
    vector<Shape> input_shapes_;
    vector<Shape> output_shapes_;
};

SPACE_END