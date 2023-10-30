#include "inst_manager.h"
#include "arch/inst_pool.hpp"
#include "base_instance.h"
#include "com/logger.h"
#include "cv_server/error_code.h"
#include "cv_server/message.h"
#include "instance/common_inst.h"
#include <cstddef>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

InstManager* InstManager::inst_mgr_ = nullptr;

static int inst_factory(TaskType type, InstParamType* param, InstanceBase** inst_ptr)
{
    switch (type)
    {
    case TaskType::COMMON:
    {
        *inst_ptr = new CommonInst();
        break;
    }
    case TaskType::SCENE_ANALYSIS:
    {
        *inst_ptr = new CommonInst();
        break;
    }
    default:
        LError("InstManager::create_inst error, no supported such type. tyep={}", type);
        return ERR_INVALID_PARAM;
    }
    int ret = (*inst_ptr)->init(*param);
    return ret;
}

int InstManager::init(const std::string& manager_param)
{
    LInfo("InstManager initialize start...");
    task_inst_pool_.clear();
    // 分类任务参数
    InferEngineParam infer_param;
    infer_param.onnx_path = "../models/mobilenetv2-12/mobilenetv2-12-int8.onnx";
    infer_param.thread_num = 1;
    infer_param.dev_type = CPU;
    infer_param.dev_id = 0;
    InstParamType infer_param_map = {std::make_pair("Classify", infer_param)};
    inst_param_map_.insert(std::make_pair(TaskType::COMMON, infer_param_map));
    // 检测任务参数
    InferEngineParam infer_param2;
    infer_param2.onnx_path = "../models/yolov3_tiny/tiny-yolov3-11.onnx";
    infer_param2.thread_num = 1;
    infer_param2.dev_type = CPU;
    infer_param2.dev_id = 0;
    InstParamType infer_param_map2 = {std::make_pair("Detection", infer_param2)};
    inst_param_map_.insert(std::make_pair(TaskType::SCENE_ANALYSIS, infer_param_map2));
    // 分割任务参数 TODO

    LInfo("InstManager initialize success...");
    return 0;
}

int InstManager::create_inst(TaskType type, InstParamType* param, int num)
{
    if (param == nullptr)
    {
        if (inst_param_map_.find(type) == inst_param_map_.end())
        {
            LError("InstManager::create_inst error, no such type. tyep={}", type);
            return ERR_INVALID_PARAM;
        }
        param = &inst_param_map_[type];
    }
    if (task_inst_pool_.find(type) == task_inst_pool_.end())
    {
        InstPool* inst_pool_ptr = new InstPool();
        int ret = inst_pool_ptr->init("info");
        task_inst_pool_.insert(std::make_pair(type, inst_pool_ptr));
    }
    for (int i = 0; i < num; ++i)
    {
        InstanceBase* inst_ptr = nullptr;
        int ret = inst_factory(type, param, &inst_ptr);
        log_error_return(ret, "InstManager::create_inst error, inst_factory failed. ret={}", ERR_CREATE_INSTANCE_FAILED);
        task_inst_pool_[type]->add_inst(inst_ptr);
    }
    return 0;
}

int InstManager::destroy_inst(TaskType task_type)
{
    if (task_inst_pool_.find(task_type) == task_inst_pool_.end())
    {
        LError("InstManager::destroy_inst error, no such type. tyep={}", task_type);
        return ERR_INVALID_PARAM;
    }
    task_inst_pool_[task_type]->fini();
    task_inst_pool_.erase(task_type);
    return 0;
}

int InstManager::run(TaskType task_type, std::vector<cv::Mat>& input_imgs, void* result)
{
    if (task_inst_pool_.find(task_type) == task_inst_pool_.end())
    {
        LError("InstManager::run error, no such type. tyep= {} ", task_type);
        return ERR_INVALID_PARAM;
    }
    InstanceBase* inst_ptr = nullptr;
    int ret = task_inst_pool_[task_type]->pull_inst(&inst_ptr);
    log_error_return(ret, "InstManager::run error, inst_pool pull_inst failed. ret={}", ret);
    do
    {
        ret = inst_ptr->compute(input_imgs, result);
        if (ret != 0)
        {
            LError("InstManager::run error, compute failed. ret={}", ret);
            break;
        }
    } while (0);
    int ret_state = task_inst_pool_[task_type]->push_inst(inst_ptr);
    if (ret_state)
    {
        LError("InstManager::run error, inst_pool push_inst failed. ret={}", ret_state);
    }
    return ret;
}

int InstManager::fini()
{
    for (auto& task_inst : task_inst_pool_)
    {
        int ret = task_inst.second->fini();
        log_error_return(ret, "InstManager::fini error, inst_pool fini failed. ret={}", ret);
        delete task_inst.second;
    }
    task_inst_pool_.clear();
    return 0;
}
