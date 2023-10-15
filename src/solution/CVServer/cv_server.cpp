#include "CVServer/cv_server.h"
#include <thread>

int CVServer::init(std::string cfg_path){
    InstMgr->init(cfg_path);
    Instance* inst_ptr = nullptr;
    InferEngineParam infer_param;
    infer_param.onnx_path = "../res/mnist.onnx";
    infer_param.thread_num = 1;
    infer_param.dev_type = CPU;
    infer_param.dev_id = 0;
    std::vector<std::pair<std::string,InferEngineParam>> infer_param_map = {std::make_pair("Classify", infer_param)};
    InstMgr->create_inst(FID::CLASSIFY, infer_param_map, &inst_ptr);
    if (inst_ptr == nullptr){
        std::cout << "create inst error" << std::endl;
        return -1;
    }

    Inst_map_.insert(std::make_pair(FID::CLASSIFY, inst_ptr));
    return 0;
}

int CVServer::process(message* msg){
    InData* in_data = (InData*)msg->input;
    OutData* out_data = (OutData*)msg->output;
    std::vector<cv::Mat> input_imgs = {in_data->img};

    Instance* inst = Inst_map_[FID::CLASSIFY];
    InstMgr->run(inst, input_imgs, (void*)&out_data->output_info);
    return 0;
}