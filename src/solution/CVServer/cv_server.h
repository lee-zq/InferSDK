#include "CVServer/message.h"
#include "common/define.hpp"
#include <thread>
#include "arch/manager.h"
#include <queue>
#include <mutex>
#include <condition_variable>

class CVServer{
public:
    virtual int init(std::string cfg_path){
        InstMgr->init(cfg_path);
    }
    // virtual int fini();
    // virtual int start();
    // virtual int stop();
    virtual int process(message* msg);
private:
    // 任务队列
    std::queue<message*> task_queue_;
    // 任务队列锁
    std::mutex task_queue_mutex_;
    // 任务队列条件变量
    std::condition_variable task_queue_cond_;
    // 线程池
    std::vector<std::thread> thread_pool_;
};