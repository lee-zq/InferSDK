#include "autosar.h"
#include <thread>

void func1(message* msg){
    std::cout<<"enter func1"<<std::endl;
    int count=0;
    while (1)
    {
    count++;
    std::cout << "count:" << count<< std::endl;
    std::cout << "msg::output:" << msg->output<< std::endl;
    }
    
}
int Autosar::process(message* msg){
    std::cout << "msg::pid" << msg->pid << std::endl;
    std::cout << "msg::fid" << msg->fid << std::endl;
    
    std::cout << "Autosar::process" << std::endl;
    std::thread t;
    // 通过switch case执行不同的打印和计算
    switch (msg->fid)
    {
    case 1:
        std::cout << "Autosar::process::fid::1" << std::endl;
        t=std::thread(func1, msg);
        msg->output = msg->input + msg->fid;
        break;
    case 2:
        std::cout << "Autosar::process::fid::2" << std::endl;
        msg->output = msg->input + msg->fid;
        break;
    case 3:
        std::cout << "Autosar::process::fid::3" << std::endl;
        msg->output = msg->input + msg->fid;
        break;
    case 4:
        std::cout << "Autosar::process::fid::4" << std::endl;
        msg->output = msg->input + msg->fid;
        break;
    default:
        std::cout << "Autosar::process::fid::error, no supported fid:" << msg->fid << std::endl;
    }
    t.join();
    return 0;
}