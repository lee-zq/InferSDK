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
    // 1. 初始化CVServer
    CVServer* cv_server = new CVServer();
    if (cv_server == nullptr)
    {
        cout << "create cv_server error" << endl;
        return -1;
    }
    int ret = cv_server->init("../config/cv_server.cfg");
    if (ret != 0)
    {
        cout << "init cv_server error" << endl;
        delete cv_server;
        return -1;
    }

    // 2. 创建Client
    Client client;
    client.init(cv_server);

    // 3. 批量处理图片
    string input_data_path =
        "../data/voc.lst"; // 存储格式 /path/to/image task_type id
    ret = client.process(input_data_path);
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
