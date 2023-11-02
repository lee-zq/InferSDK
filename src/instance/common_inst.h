#pragma once

#include "base_instance.h"
#include "base_type.h"
#include "com/define.h"
#include "cv_server/message.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

SPACE_BEGIN

typedef struct _CommonInstDataTree
{
    cv::Mat input_mat;
    ClassInfo class_info;
    std::vector<DetectInfo> detect_info;
    std::vector<SegInfo> seg_info;
} CommonInstDataTree;

class CommonInst : public InstanceBase
{
public:
    virtual int init(const InstParamType& param) override;
    virtual int compute(cv::Mat* input_data, void* output_data) override;
    virtual int fini() override;
    virtual ~CommonInst(){};

private:
    void set_input(const cv::Mat& input_data);
    void set_output(void* output_data);
    CommonInstDataTree data_mgr_;
};

SPACE_END