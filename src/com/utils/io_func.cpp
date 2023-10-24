#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "com/utils/base_func.h"

std::vector<std::string> load_file(const std::string& file_path)
{
    std::ifstream f(file_path.c_str());
    if (!f.is_open())
    {
        std::cout << "open file failed" << std::endl;
        return {};
    }
    std::vector<std::string> data_lst;
    std::string item;
    while (std::getline(f, item))
    {
        if (item.size() > 0)
        {
            data_lst.push_back(item);
        }
    }
    f.close();
    return data_lst;
}

int save_result(std::string& file_path, std::vector<int>& results)
{
    std::ofstream f(file_path.c_str());
    if (!f.is_open())
    {
        std::cout << "open file failed" << std::endl;
        return -1;
    }
    for (auto& res : results)
    {
        f << res << std::endl;
    }
    f.close();
    return 0;
}


// split string by delim
int split(const std::string& str,
                 std::vector<std::string>& ret_,
                 std::string sep)
{
    if (str.empty())
    {
        return 0;
    }
    ret_.clear();
    std::string tmp;
    std::string::size_type pos_begin = str.find_first_not_of(sep);
    std::string::size_type comma_pos = 0;
    while (pos_begin != std::string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != std::string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }
        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

int Bbox2Json(std::vector<Bbox>& bbox_vec, std::string& json_str)
{
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (auto& bbox : bbox_vec)
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("x1", bbox.x1, allocator);
        obj.AddMember("y1", bbox.y1, allocator);
        obj.AddMember("x2", bbox.x2, allocator);
        obj.AddMember("y2", bbox.y2, allocator);
        obj.AddMember("class_id", bbox.class_id, allocator);
        obj.AddMember("score", bbox.score, allocator);
        array.PushBack(obj, allocator);
    }
    doc.AddMember("bbox", array, allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    json_str = buffer.GetString();
    // std::cout << json_str << std::endl;
    return 0;
}