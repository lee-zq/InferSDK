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

int CommonInst::compute(std::vector<cv::Mat>& input_imgs, void* results)
{
    for (auto& module : modules_)
    {
        int ret = module.second->inference(input_imgs, results);
        if (ret != 0)
        {
            LError("CommonInst::compute error, module inference failed. ret={}", ret);
            return -1;
        }
    }
    return 0;
}

int CommonInst::fini()
{
    for_each(modules_.begin(), modules_.end(), [](const std::pair<ModuleType, std::shared_ptr<ModuleBase>>& it) {
        it.second->uninit();
    });
    modules_.clear();
    return 0;
}