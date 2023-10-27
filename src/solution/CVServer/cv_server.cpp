#include "cv_server/cv_server.h"
#include "arch/inst_manager.h"
#include "interface/base_instance.h"
#include "com/logger.h"
#include <cstddef>
#include <thread>

int CVServer::init(std::string cfg_path)
{
    LInfo("CVServer Initialize start.");
    // 1. 初始化线程池
    thread_pool_ = std::make_shared<ThreadPool>(1);
    thread_pool_->init();
    // 2. 初始化实例管理器
    int ret = InstMgr->init(cfg_path);
    log_error_return(ret, "CVServer Initialize failed.", ret);
    initialized_ = true;
    LInfo("CVServer Initialize success.");
    return 0;
}

int CVServer::create_inst(TaskType task_type, int num)
{
    InstParamType* inst_param = nullptr;
    int ret = InstMgr->create_inst(task_type, inst_param, 1);
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