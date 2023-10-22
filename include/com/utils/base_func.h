#pragma once
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static float cal_acc(std::vector<int>& labels, std::vector<int>& results);

std::vector<std::string> load_file(const std::string& file_path);

int save_result(std::string& file_path, std::vector<int>& results);

int split(const std::string& str,
          std::vector<std::string>& ret_,
          std::string sep = ",");

void softmax(std::vector<float>& data_vec);

void sigmoid(std::vector<float>& data_vec);