#include "arch/inst_manager.h"
#include "classification/classifier.h"
#include "cv_server/error_code.h"
#include "cv_server/message.h"
#include <string>
#include <vector>

InstManager* InstManager::inst_mgr_ = nullptr;

int InstManager::init(const std::string& manager_param)
{
    LInfo("InstManager initialize start...");
    // 分类任务参数
    InferEngineParam infer_param;
    infer_param.onnx_path = "../res/mobilenetv2-12/mobilenetv2-12-int8.onnx";
    infer_param.thread_num = 1;
    infer_param.dev_type = CPU;
    infer_param.dev_id = 0;
    InstParamType infer_param_map = {std::make_pair("Classify", infer_param)};
    inst_param_map_.insert(std::make_pair(TaskType::CLASSIFY, infer_param_map));
    // 检测任务参数
    InferEngineParam infer_param2;
    infer_param2.onnx_path = "../res/yolov3_tiny/tiny-yolov3-11.onnx";
    infer_param2.thread_num = 1;
    infer_param2.dev_type = CPU;
    infer_param2.dev_id = 0;
    InstParamType infer_param_map2 = {
        std::make_pair("Detection", infer_param2)};
    inst_param_map_.insert(
        std::make_pair(TaskType::DETECTION, infer_param_map2));
    // 分割任务参数 TODO

    LInfo("InstManager initialize success...");
    return 0;
}

int InstManager::create_inst(TaskType type, Instance** inst_ptr)
{
    if (inst_param_map_.find(type) == inst_param_map_.end())
    {
        LError("InstManager::create_inst error, no such type. tyep=%d", type);
        return ERR_INVALID_PARAM;
    }
    InstParamType param = inst_param_map_[type];
    Instance* created_inst = new Instance();
    int ret = created_inst->init(param);
    if (ret != 0)
    {
        LError("InstManager::create_inst error, inst init "
               "failed.") return ERR_CREATE_INSTANCE_FAILED;
    }
    *inst_ptr = created_inst;
    return 0;
}

int InstManager::destroy_inst(Instance* inst_ptr)
{
    delete inst_ptr;
    return 0;
}

int InstManager::run(Instance* inst_ptr,
                     std::vector<cv::Mat>& input_imgs,
                     void* result)
{
    inst_ptr->compute(input_imgs, result);
    return 0;
}

int InstManager::fini()
{
    return 0;
}
