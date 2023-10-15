#pragma once
#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <vector>

#include "onnxruntime_cxx_api.h"
#include "opencv2/opencv.hpp"
#include "tensor.h"

#include "engine/infer_engine.h"

using namespace std;

class ORTEngine : public InferEngine{
 public:
    ORTEngine(){};
    virtual int init(const InferEngineParam& param) override;
    virtual int forward(const vector<Tensor>& input_data, vector<Tensor>& output_data) override;
    virtual int async_forward(const vector<Tensor>& input_data, vector<Tensor>& output_data) override{
      return 0;
    }
    virtual int uninit(){
      if (session_){
        delete session_;      
      }
      if (allocator_){
        delete allocator_;
      }
      return 0;
    };
private:
    virtual int bind_io_data(const vector<Tensor>& input_data, vector<Tensor>& output_data) ;

public:
    InferEngineParam param_;
    Ort::MemoryInfo memory_info_{nullptr};
    Ort::AllocatorWithDefaultOptions*  allocator_{nullptr};

    vector<Ort::Value> input_tensor_;
    vector<Ort::Value> output_tensor_;
  
private:
    Ort::SessionOptions session_option;
    Ort::Env env{ORT_LOGGING_LEVEL_WARNING, "onnxruntime"};
    Ort::Session* session_{nullptr};
};