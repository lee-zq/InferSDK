#include "cv_server/cv_server.h"
#include "arch/inst_manager.h"
#include "interface/base_instance.h"
#include "com/logger.h"
#include <cstddef>
#include <thread>
#include "com/conf_reader.h"

int CVServer::init(std::string cfg_path)
{
    int ret = parser_cfg(cfg_path);
    log_error_return(ret, "CVServer Initialize parser_cfg() failed.", ret);
    initialized_ = true;
    LInfo("CVServer Initialize success.");
    return 0;
}

int CVServer::parser_cfg(const std::string& cfg_path)
{
    ConfReader conf(cfg_path);
    conf.setSection("CVServer");

    // 初始化执行线程池
    int thread_pool_size = conf.readInt("thread_pool_size", 1);
    thread_pool_ = std::make_shared<ThreadPool>(thread_pool_size);
    thread_pool_->init();

    // 初始化Log
    std::string log_file = conf.readStr("log_file", "./InferSDK.log");
    LInit(log_file.c_str());

    // 初始化实例管理器
    std::string module_cfg = conf.readStr("module_cfg", "../cfg/module.cfg");
    int ret = InstMgr->init(module_cfg);
    log_error_return(ret, "CVServer Initialize failed.", ret);
    return 0;
}

int CVServer::create_inst(TaskType task_type, int num)
{
    int ret = InstMgr->create_inst(task_type, num);
    log_error_return(ret, "CVServer::process error", ret);
    return 0;
}

int CVServer::destroy_inst(TaskType task_type)
{
    int ret = InstMgr->destroy_inst(task_type);
    log_error_return(ret, "CVServer::process error", ret);
    return 0;
}

int CVServer::process(message* msg)
{
    InData* in_data = (InData*)msg->input;
    OutData* out_data = (OutData*)msg->output;
    std::vector<cv::Mat> input_imgs = {in_data->img};
    TaskType task_type = static_cast<TaskType>(msg->task_type);

    auto task = [&]() {
        int ret = InstMgr->run(task_type, input_imgs, (void*)&out_data->output_info);
        return ret;
    };
    std::future<int> result = thread_pool_->submit(task);
    if (result.get() != 0)
    { // 阻塞等待
        LError("CVServer::process error");
        return ERR_GENERAL;
    }
    return ERR_SUCCESS;
}

int CVServer::fini()
{
    thread_pool_->shutdown();
    InstMgr->fini();
    return 0;
}