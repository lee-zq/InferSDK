#include "cv_server/cv_server.h"
#include <thread>

int CVServer::init(std::string cfg_path){
    // 1. 初始化线程池
    thread_pool_ = std::make_shared<ThreadPool>(0);
    thread_pool_->init();
    // 2. 初始化实例管理器
    InstMgr->init(cfg_path);
    LInfo("CVServer Initialize success.")
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
    auto task = [&](){
        return inst->compute(input_imgs, (void*)&out_data->output_info);
    };
    // 异步执行任务可分为三种方式：
    // 1. 使用线程池
    // std::future<int> ret = thread_pool_->submit(task);
    // 2. 直接创建线程
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::cout << "thread id: " << ss.str() << std::endl;
    std::thread t(task);
    t.join();

    // 3. 启动异步线程并获取返回值
    // std::future<int> ret = std::async(task);
    // if (ret.get() != 0){ // 这里要阻塞等待
    //     LError("CVServer::process error");
    //     return -1;
    // }
    return 0;
}

int CVServer::fini(){
    thread_pool_->shutdown();
    InstMgr->fini();
    return 0;
}