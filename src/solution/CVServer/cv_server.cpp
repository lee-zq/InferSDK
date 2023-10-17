#include "cv_server/cv_server.h"
#include <thread>

int CVServer::init(std::string cfg_path){
    // 1. 初始化线程池
    thread_pool_ = std::make_shared<ThreadPool>(4);
    thread_pool_->init();
    // 2. 初始化实例管理器
    InstMgr->init(cfg_path);
    LInfo("CVServer Initialize success")
    return 0;
}

int CVServer::process(message* msg){
    InData* in_data = (InData*)msg->input;
    OutData* out_data = (OutData*)msg->output;
    std::vector<cv::Mat> input_imgs = {in_data->img};
    FID fid = static_cast<FID>(msg->fid);
    if (Inst_map_.find(fid) == Inst_map_.end()){
        // 创建实例
        Instance* inst_ptr = nullptr;
        int ret = InstMgr->create_inst(fid, &inst_ptr);
        if (ret != 0 || inst_ptr == nullptr){
            LError("CVServer::process error");
            return -1;
        }
        Inst_map_.insert(std::make_pair(fid, inst_ptr));
    }
    Instance* inst = Inst_map_[fid];
    std::future<int> ret = thread_pool_->submit(
        std::bind(&Instance::compute, inst, input_imgs, (void*)&out_data->output_info)
    );
    if (ret.get() != 0){ // 这里要阻塞等待
        LError("CVServer::process error");
        return -1;
    }
    return 0;
}

int CVServer::fini(){
    thread_pool_->shutdown();
    InstMgr->fini();
    return 0;
}