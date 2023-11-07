#include "inst_manager.h"
#include "all_type.h"
#include "arch/inst_pool.hpp"
#include "base_instance.h"
#include "com/define.h"
#include "com/logger.h"
#include "com/utils/conf_reader.h"
#include "cv_server/error_code.h"
#include "cv_server/message.h"
#include "instance/common_inst.h"
#include <cstddef>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

SPACE_BEGIN
InstManager* InstManager::inst_mgr_ = nullptr;

static TaskType task_type_map(std::string task_type)
{
    static std::unordered_map<std::string, TaskType> task_type_map = {{"COMMON", TaskType::COMMON},
                                                                      {"SCENE_ANALYSIS", TaskType::SCENE_ANALYSIS}};
    auto iter = task_type_map.find(task_type);
    if (iter == task_type_map.end())
    {
        LError("InstManager::task_type_map error, no supported such type. tyep={}", task_type);
        return TaskType::INVALID_TYPE;
    }
    return iter->second;
}

static ModuleType module_type_map(std::string module_type)
{
    static std::unordered_map<std::string, ModuleType> module_type_map = {{"Classification", ModuleType::Classification},
                                                                          {"Detection", ModuleType::Detection},
                                                                          {"Segmentation", ModuleType::Segmentation}};
    auto iter = module_type_map.find(module_type);
    if (iter == module_type_map.end())
    {
        LError("InstManager::module_type_map error, no supported such type. tyep={}", module_type);
        return ModuleType::InvalidType;
    }
    return iter->second;
}

static DeviceType dev_type_map(std::string dev_type)
{
    static std::unordered_map<std::string, DeviceType> dev_type_map = {{"CPU", DeviceType::CPU},
                                                                       {"CUDA", DeviceType::CUDA},
                                                                       {"NPU", DeviceType::NPU},
                                                                       {"OTHER", DeviceType::OTHER}};
    auto iter = dev_type_map.find(dev_type);
    if (iter == dev_type_map.end())
    {
        LError("InstManager::dev_type_map error, no supported such type. tyep={}", dev_type);
        return DeviceType::OTHER;
    }
    return iter->second;
}


static int inst_factory(TaskType type, const InstParamType& param, InstanceBase** inst_ptr)
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
        LError("InstManager::create_inst error, no supported such type. tyep={}", (int)type);
        return ERR_INVALID_PARAM;
    }
    int ret = (*inst_ptr)->init(param);
    return ret;
}

int InstManager::init(const std::string& inst_mgr_cfg)
{
    LInfo("InstManager initialize start...");
    task_inst_pool_.clear();
    // 分割任务参数 TODO
    parser_cfg(inst_mgr_cfg);
    LInfo("InstManager initialize success...");
    return 0;
}

int InstManager::parser_cfg(std::string cfg_path)
{
    ConfReader cfg(cfg_path);
    cfg.setSection("Global");
    std::vector<std::string> enabled_task_type_vec = cfg.readStrArray("enabled_task_type_names");
    int default_dev_id = cfg.readInt("dev_id", 0);
    std::string default_dev_type = cfg.readStr("dev_type", "CPU");
    int default_thread_num = cfg.readInt("thread_num", 1);
    for (auto& task_name : enabled_task_type_vec)
    {
        cfg.setSection(task_name);
        InstParamType inst_param;
        inst_param.type = task_name;
        inst_param.dev_id = cfg.readInt("dev_id", default_dev_id);
        std::string inst_dev_type = cfg.readStr("dev_type", default_dev_type);
        inst_param.dev_type = dev_type_map(inst_dev_type);
        inst_param.thread_num = cfg.readInt("thread_num", default_thread_num);
        std::vector<std::string> module_name_vec = cfg.readStrArray("module");
        for (auto& module_name : module_name_vec)
        {
            cfg.setSection(module_name);
            ModuleParamType module_param;
            module_param.type = module_name;
            module_param.res_path = cfg.readStr("res_path", "");
            module_param.thread_num = cfg.readInt("thread_num", inst_param.thread_num);
            module_param.dev_id = cfg.readInt("dev_id", inst_param.dev_id);
            module_param.dev_type = dev_type_map(cfg.readStr("dev_type", inst_dev_type));
            inst_param.module_params.push_back(std::make_pair(module_type_map(module_param.type), module_param));
        }
        inst_param_map_.insert(std::make_pair(task_type_map(task_name), inst_param));
    }
    return 0;
}

int InstManager::create_inst(TaskType type, int num)
{
    if (task_inst_pool_.find(type) == task_inst_pool_.end())
    {
        InstPool* inst_pool_ptr = new InstPool();
        int ret = inst_pool_ptr->init("info");
        task_inst_pool_.insert(std::make_pair(type, inst_pool_ptr));
    }
    for (int i = 0; i < num; ++i)
    {
        InstanceBase* inst_ptr = nullptr;
        int ret = inst_factory(type, inst_param_map_[type], &inst_ptr);
        log_error_return(ret, "InstManager::create_inst error, inst_factory failed. ret={}", ERR_CREATE_INSTANCE_FAILED);
        task_inst_pool_[type]->add_inst(inst_ptr);
    }
    return 0;
}

int InstManager::destroy_inst(TaskType task_type)
{
    if (task_inst_pool_.find(task_type) == task_inst_pool_.end())
    {
        LError("InstManager::destroy_inst error, no such type. tyep={}", (int)task_type);
        return ERR_INVALID_PARAM;
    }
    task_inst_pool_[task_type]->fini();
    task_inst_pool_.erase(task_type);
    return 0;
}

int InstManager::run(TaskType task_type, cv::Mat* input_data, void* output_data)
{
    if (task_inst_pool_.find(task_type) == task_inst_pool_.end())
    {
        LError("InstManager::run error, no such type. tyep= {} ", (int)task_type);
        return ERR_INVALID_PARAM;
    }
    InstanceBase* inst_ptr = nullptr;
    int ret = task_inst_pool_[task_type]->pull_inst(&inst_ptr);
    log_error_return(ret, "InstManager::run error, inst_pool pull_inst failed. ret={}", ret);
    do
    {
        ret = inst_ptr->compute(input_data, output_data);
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

SPACE_END