#pragma once
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../../all_type.h"
#include "define.h"
#include "rapidjson/filereadstream.h"   //文件输入流
#include "rapidjson/filewritestream.h"  //文件输出流
#include "rapidjson/prettywriter.h"     //文件格式化输出
#include "rapidjson/pointer.h"
#include "rapidjson/document.h"

static float cal_acc(std::vector<int>& labels, std::vector<int>& results);

std::vector<std::string> load_file(const std::string& file_path);

int save_result(std::string& file_path, std::vector<int>& results);

int split(const std::string& str, std::vector<std::string>& ret, std::string sep = ",");

void softmax(std::vector<float>& data_vec);

void sigmoid(std::vector<float>& data_vec);