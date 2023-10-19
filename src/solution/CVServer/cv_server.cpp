#include "cv_server/cv_server.h"
#include <thread>

int CVServer::init(std::string cfg_path){
    // 1. 初始化线程池
    thread_pool_ = std::make_shared<ThreadPool>(1);
    thread_pool_->init();
    // 2. 初始化实例管理器
    int ret = InstMgr->init(cfg_path);
    log_error_return(ret, "CVServer Initialize success.")
    return 0;
}

int CVServer::get_inst(FID fid, Instance** inst_ptr){
    std::lock_guard<std::mutex> lock(inst_map_mutex_);
    if (inst_map_.find(fid) == inst_map_.end()){
        // 创建实例
        Instance* inst_ptr = nullptr;
        int ret = InstMgr->create_inst(fid, &inst_ptr);
        log_error_return(ret, "CVServer::process error")
        inst_map_.insert(std::make_pair(fid, inst_ptr));
    }
    *inst_ptr = inst_map_[fid];
    return 0;
}

int CVServer::process(message* msg){
    InData* in_data = (InData*)msg->input;
    OutData* out_data = (OutData*)msg->output;
    std::vector<cv::Mat> input_imgs = {in_data->img};
    FID fid = static_cast<FID>(msg->fid);
    Instance* inst = nullptr;
    int ret = get_inst(fid, &inst);
    log_error_return(ret, "CVServer::process get_inst() failed")
    auto task = [&](){
        return inst->compute(input_imgs, (void*)&out_data->output_info);
    };

    std::future<int> result = thread_pool_->submit(task);
    if (result.get() != 0){ // 阻塞等待
        LError("CVServer::process error");
        return ERR_GENERAL;
    }
    return ERR_SUCCESS;
}

int CVServer::fini(){
    thread_pool_->shutdown();
    InstMgr->fini();
    return 0;
}