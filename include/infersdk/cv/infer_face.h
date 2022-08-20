#pragma once
#include <string>
#include <vector>
#include "data_type.h"


class InferFace
{
public:
    InferFace(/* args */);
    virtual int init() = 0;
    virtual int forward() = 0;
    virtual int finish() = 0;
    virtual ~InferFace();

private:
    /* data */
};

InferFace::InferFace(/* args */)
{
}

InferFace::~InferFace()
{
}
