#include "cv_server/cv_server.h"
#include "opencv2/opencv.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <time.h>
// #include "cv_server/cv_server.h"
#include "client.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Error! \n Usage: ./test_cvserver client.cfg cv_server.cfg" << std::endl;
        return -1;
    }
    // 0. 命令行参数解析
    std::string client_cfg = argv[1];
    std::string cv_server_cfg = argv[2];
    // 1. 初始化CVServer
    CVServer* cv_server = new CVServer();
    if (cv_server == nullptr)
    {
        cout << "create cv_server error" << endl;
        return -1;
    }
    int ret = cv_server->init(cv_server_cfg);
    if (ret != 0)
    {
        cout << "init cv_server error" << endl;
        delete cv_server;
        return -1;
    }

    // 2. 创建Client
    Client client;
    ret = client.init(client_cfg);
    if (ret != 0)
    {
        cout << "init client error" << endl;
        return -1;
    }
    ret = client.start(cv_server);
    if (ret != 0)
    {
        cout << "start client error" << endl;
        return -1;
    }
    // 推理配置文件client.cfg中的data_lst指定的数据
    ret = client.process();
    if (ret != 0)
    {
        cout << "main() process error" << endl;
        return -1;
    }
    // 推理外部指定的数据
    string input_data_path = "../data/voc.lst"; // 存储格式 /path/to/image task_type req_id
    ret = client.process(input_data_path);
    if (ret != 0)
    {
        cout << "main() process error" << endl;
        return -1;
    }
    client.stop();
    // 4. 销毁CVServer
    ret = cv_server->fini();
    delete cv_server;
    if (ret != 0)
    {
        cout << "fini cv_server error" << endl;
        return -1;
    }

    return 0;
}
