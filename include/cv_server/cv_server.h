#pragma once
#include "arch/inst_manager.h"
#include "com/define.h"
#include "cv_server/message.h"
#include "error_code.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "com/threadpool/threadpool.hpp"

SPACE_BEGIN

class CVServer
{
public:
    int init(std::string cfg_path);
    int parser_cfg(const std::string& cfg_path);
    int fini();
    // int start();
    // int stop();
    int create_inst(TaskType task_type, int num = 1);
    int destroy_inst(TaskType task_type);
    int process(message* msg);

private:
    bool initialized_ = false;
    // std::map<TaskType, InstanceBase*> inst_map_;
    // std::mutex inst_map_mutex_;
    // 执行线程池
    int thread_pool_size_ = 1;
    std::shared_ptr<ThreadPool> thread_pool_;
};

SPACE_END