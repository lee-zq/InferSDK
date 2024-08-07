#include "cv_server/cv_server_api.h"
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
    void* cv_server = nullptr;
    int ret = CreateCVServer(&cv_server);
    if (ret != 0)
    {
        cout << "create cv_server error" << endl;
        return -1;
    }
    ret = InitCVServer(cv_server, "../config/cv_server.cfg");
    if (ret != 0)
    {
        cout << "init cv_server error" << endl;
        return -1;
    }

    // 2. 创建Client
    Client client;
    client.init(cv_server);

    // 3. 发送图片
    string img_path = "../data/test.lst";
    int task_type = 0;
    int id = 0;
    ret = client.process(img_path);
    if (ret != 0)
    {
        cout << "client send error" << endl;
        return -1;
    }

    // 4. 销毁CVServer
    ret = DestroyCVServer(cv_server);
    if (ret != 0)
    {
        cout << "destroy cv_server error" << endl;
        return -1;
    }

    return 0;
}
