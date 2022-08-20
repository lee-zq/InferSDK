#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>

#include "cv_inst.h"

CVInst::CVInst(const char* onnx_path, dev_t device_type) 
  : InferFace(),
  allocator_info_(Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU))
{
  session_option.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
  session_option.SetExecutionMode(ExecutionMode::ORT_PARALLEL);
  OrtSessionOptionsAppendExecutionProvider_CUDA(session_option, device_type);
  session = Ort::Session(env, onnx_path, session_option);
}


int CVInst::forward() {
  int ret = preproc();
  ret = compute();
  ret = postproc();
  return 0;
}
