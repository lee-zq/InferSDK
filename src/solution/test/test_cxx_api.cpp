#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "cv_server/cv_server.h"
// #include "cv_server/cv_server.h"
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
    // string img_path = "../data/mnist_img/0.png";
    // int fid = 1;
    // int pid = 10000;
    // cv::Mat input_img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
    // ret = client.process(input_img, fid, pid);
    // if(ret != 0){
    //     cout << "client send error" << endl;
    //     return -1;
    // } 
    // 4. 批量处理
    string input_data_path = "../data/voc.lst"; // 存储格式 /path/to/image fid pid
    ret = client.process(input_data_path);
    // 4. 销毁CVServer
    ret = cv_server->fini();
    delete cv_server;
    if(ret != 0){
        cout << "fini cv_server error" << endl;
        return -1;
    }
    return 0;
}
