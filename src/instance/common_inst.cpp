#include "common_inst.h"

#include "all_type.h"
#include "arch/module_factory.hpp"
#include "base_module.h"
#include "com/logger.h"
#include "com/utils/base_func.h"
#include "cv_server/error_code.h"
#include "cv_server/message.h"
#include "module/classification/classifier.h"
#include "module/detection/detection.h"
#include "module/segmentation/seg.h"
#include <algorithm>

int CommonInst::init(const InstParamType& param)
{
    for (auto& module_param : param.module_params)
    {
        auto module_ptr = ModuleRegistry::CreateModule(module_param.second);
        int ret = module_ptr->init(module_param.second);
        log_error_return(ret, "create module failed.", ERR_CREATE_MODULE_FAILED);
        modules_.emplace_back(std::make_pair(module_param.first, module_ptr));
    }
    return 0;
}

void CommonInst::set_input(const cv::Mat& input_data)
{
    data_mgr_.input_mat = input_data;
    data_mgr_.class_info.id = -1;
    data_mgr_.class_info.score = 0.f;
    data_mgr_.detect_info.clear();
    data_mgr_.seg_info.clear();
}

void CommonInst::set_output(void* output_data)
{
    auto result = static_cast<OutData*>(output_data);
}

int CommonInst::compute(cv::Mat* input_data, void* output_data)
{
    int ret = ERR_SUCCESS;
    set_input(*input_data);
    for (auto& module : modules_)
    {
        switch (module.first)
        {
        case ModuleType::Classification:
            ret = module.second->inference(data_mgr_.input_mat, &data_mgr_.class_info);
            break;
        case ModuleType::Detection:
            ret = module.second->inference(data_mgr_.input_mat, &data_mgr_.detect_info);
            break;
        case ModuleType::Segmentation:
            ret = module.second->inference(data_mgr_.input_mat, &data_mgr_.seg_info);
            break;
        default:
            LError("CommonInst::compute error, invalid module type. type={}", (int)module.first);
            ret = ERR_INVALID_MODULE;
        }
        if (ret != 0)
        {
            LError("CommonInst::compute error, module inference failed.type={} ret={} ", (int)module.first, ret);
            ret = ERR_COMPUTE_ERROR;
            break;
        }
    }
    set_output(output_data);
    return ret;
}

int CommonInst::fini()
{
    for_each(modules_.begin(), modules_.end(), [](const std::pair<ModuleType, std::shared_ptr<ModuleBase>>& it) {
        it.second->uninit();
    });
    modules_.clear();
    return 0;
}