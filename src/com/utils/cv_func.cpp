#pragma once
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "com/utils/base_func.h"

float cal_acc(std::vector<int>& labels, std::vector<int>& results)
{
    float TP = 0.;
    for (int i = 0; i < labels.size(); i++)
    {
        if (labels[i] == results[i])
        {
            TP++;
        }
    }
    return TP / labels.size();
}
