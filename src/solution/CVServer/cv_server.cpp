#include "cv_server.h"
#include <thread>

int CVServer::process(message* msg){
    std::cout << "msg::pid" << msg->pid << std::endl;
    std::cout << "msg::fid" << msg->fid << std::endl;
    
    std::cout << "CVServer::process" << std::endl;
    std::thread t;
    // 通过switch case执行不同的打印和计算
    switch (msg->fid)
    {
    case 1:
        std::cout << "CVServer::process::fid::1" << std::endl;
        // t=std::thread(func1, msg);
        msg->output = msg->input + msg->fid;
        break;
    case 2:
        std::cout << "CVServer::process::fid::2" << std::endl;
        msg->output = msg->input + msg->fid;
        break;
    case 3:
        std::cout << "CVServer::process::fid::3" << std::endl;
        msg->output = msg->input + msg->fid;
        break;
    case 4:
        std::cout << "CVServer::process::fid::4" << std::endl;
        msg->output = msg->input + msg->fid;
        break;
    default:
        std::cout << "CVServer::process::fid::error, no supported fid:" << msg->fid << std::endl;
    }
    // t.join();
    return 0;
}