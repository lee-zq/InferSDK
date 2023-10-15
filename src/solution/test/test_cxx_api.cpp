#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "CVServer/cv_server.h"
// #include "CVServer/cv_server.h"
#include "client.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv){
    // 1. 初始化CVServer
    CVServer* cv_server = new CVServer();
    if(cv_server == nullptr){
        cout << "create cv_server error" << endl;
        return -1;
    }
    int ret = cv_server->init("../config/cv_server.cfg");
    if(ret != 0){
        cout << "init cv_server error" << endl;
        return -1;
    }

    // 2. 创建Client
    Client client;
    client.init(cv_server);

    // 3. 发送图片
    string img_path = "/root/lzq/code/InferSDK/output/data/9.png";
    int fid = 0;
    int pid = 0;
    ret = client.Send(img_path, fid, pid);
    if(ret != 0){
        cout << "client send error" << endl;
        return -1;
    } 

    // 4. 销毁CVServer
    delete cv_server;

    return 0;
}
