#include "common_inst.h"

#include "all_type.h"
#include "base_module.h"
#include "com/logger.h"
#include "com/utils/base_func.h"
#include "cv_server/error_code.h"
#include "cv_server/message.h"
#include "module/classification/classifier.h"
#include "module/detection/detection.h"
#include "module/segmentation/seg.h"

static int module_factory(ModuleType module_type, const ModuleParamType& param, ModuleBase** module_ptr)
{
    switch (module_type)
    {
    case ModuleType::CLASSIFY:
        *module_ptr = new Classifier();
        break;
    case ModuleType::DETECTION:
        *module_ptr = new Detection();
        break;
    default:
        std::cout << "No supported task type : " << (int)module_type << std::endl;
    }
    int ret = (*module_ptr)->init(param);
    if (ret != 0)
    {
        std::cout << "error" << std::endl;
        return ret;
    }
    return 0;
}

int CommonInst::init(const InstParamType& param)
{

    for (auto& module_param : param.module_params)
    {
        ModuleBase* module_ptr = nullptr;
        int ret = module_factory(module_param.first, module_param.second, &module_ptr);
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
    for (auto& module : modules_)
    {
        module.second->uninit();
        delete module.second;
    }
    modules_.clear();
    return 0;
}