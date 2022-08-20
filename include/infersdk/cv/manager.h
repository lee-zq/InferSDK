#include "infer_face.h"

#include <string>
#include <vector>

class Manager
{
public:
    Manager(/* args */);
    virtual int init() = 0;
    virtual int run() = 0;
    virtual int finish() = 0;
    virtual ~Manager();

private:
    int create_inferinst(std::string task_type, InferFace** handle);

private:
    InferFace* inferInst_;
};