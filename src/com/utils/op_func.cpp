#pragma once
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "com/utils/base_func.h"

void softmax(std::vector<float>& data_vec)
{
    float sum = 0;
    for (int i = 0; i < data_vec.size(); i++)
    {
        sum += std::exp(data_vec[i]);
    }
    for (int i = 0; i < data_vec.size(); i++)
    {
        data_vec[i] = std::exp(data_vec[i]) / sum;
    }
}

void sigmoid(std::vector<float>& data_vec)
{
    for (int i = 0; i < data_vec.size(); i++)
    {
        data_vec[i] = 1.0 / (1.0 + std::exp(-data_vec[i]));
    }
}