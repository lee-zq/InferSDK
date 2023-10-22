#include "cv_server/cv_server.h"
#include "com/logger.h"
#include <thread>

int CVServer::init(std::string cfg_path)
{
    LInfo("CVServer Initialize start.");
    // 1. 初始化线程池
    thread_pool_ = std::make_shared<ThreadPool>(1);
    thread_pool_->init();
    // 2. 初始化实例管理器
    int ret = InstMgr->init(cfg_path);
    log_error_return(ret, "CVServer Initialize failed.");
    initialized_ = true;
    LInfo("CVServer Initialize success.");
    return 0;
}

int CVServer::get_inst(TaskType task_type, Instance** inst_ptr)
{
    std::lock_guard<std::mutex> lock(inst_map_mutex_);
    if (inst_map_.find(task_type) == inst_map_.end())
    {
        // 创建实例
        Instance* inst_ptr = nullptr;
        int ret = InstMgr->create_inst(task_type, &inst_ptr);
        log_error_return(ret, "CVServer::process error");
        inst_map_.insert(std::make_pair(task_type, inst_ptr));
    }
    *inst_ptr = inst_map_[task_type];
    return 0;
}

int CVServer::process(message* msg)
{
    InData* in_data = (InData*)msg->input;
    OutData* out_data = (OutData*)msg->output;
    std::vector<cv::Mat> input_imgs = {in_data->img};
    TaskType task_type = static_cast<TaskType>(msg->task_type);
    Instance* inst = nullptr;
    int ret = get_inst(task_type, &inst);
    log_error_return(ret, "CVServer::process get_inst() failed");
    auto task = [&]() {
        return inst->compute(input_imgs, (void*)&out_data->output_info);
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