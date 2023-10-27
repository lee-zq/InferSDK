#include "common_inst.h"

#include "com/utils/base_func.h"
#include "module/classification/classifier.h"
#include "module/detection/detection.h"
#include "module/segmentation/seg.h"

int CommonInst::init(const std::vector<std::pair<std::string, InferEngineParam>>& param)
{
    if (param.size() != 1)
    {
        LError("inst_init_param.size() != 1 current supported one module");
        return -1;
    }
    for (int i = 0; i < param.size(); i++)
    {
        append_module(param[i].first, param[i].second);
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
            LError("CommonInst::compute error, module inference failed. ret=%d", ret);
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

int CommonInst::append_module(const std::string& module_type, const InferEngineParam& param)
{
    ModuleBase* module_ptr = nullptr;
    if (module_type == "Classify")
    {
        module_ptr = new Classifier();
        int ret = module_ptr->init(param);
        if (ret != 0)
        {
            std::cout << "error" << std::endl;
            return ret;
        }
    }
    else if (module_type == "Detection")
    {
        module_ptr = new Detection();
        int ret = module_ptr->init(param);
        if (ret != 0)
        {
            std::cout << "error" << std::endl;
            return ret;
        }
    }
    else
    {
        std::cout << "No supported task type : " << module_type << std::endl;
    }
    modules_.emplace_back(std::make_pair(module_type, module_ptr));
    return 0;
}
