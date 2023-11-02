#pragma once
#include "all_type.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"  //文件输入流
#include "rapidjson/filewritestream.h" //文件输出流
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h" //文件格式化输出

inline rapidjson::Value ClassInfo2JsonValue(ClassInfo& data, rapidjson::Document::AllocatorType& allocator)
{
    rapidjson::Value obj(rapidjson::kObjectType);
    obj.AddMember("id", data.id, allocator);
    obj.AddMember("score", data.score, allocator);
    obj.AddMember("name", rapidjson::Value(data.name.c_str(), data.name.size(), allocator), allocator);
    return obj;
}

inline rapidjson::Value DetectInfo2JsonValue(DetectInfo& data, rapidjson::Document::AllocatorType& allocator)
{
    rapidjson::Value obj(rapidjson::kObjectType);
    obj.AddMember("id", data.id, allocator);
    obj.AddMember("score", data.score, allocator);
    obj.AddMember("name", rapidjson::Value(data.name.c_str(), data.name.size(), allocator), allocator);

    rapidjson::Value bbox(rapidjson::kObjectType);
    bbox.AddMember("x1", data.bbox.x1, allocator);
    bbox.AddMember("y1", data.bbox.y1, allocator);
    bbox.AddMember("x2", data.bbox.x2, allocator);
    bbox.AddMember("y2", data.bbox.y2, allocator);
    
    obj.AddMember("bbox", bbox, allocator);
    return obj;
}

inline rapidjson::Value SegInfo2JsonValue(SegInfo& data, rapidjson::Document::AllocatorType& allocator)
{
    rapidjson::Value obj(rapidjson::kObjectType);
    obj.AddMember("id", data.id, allocator);
    obj.AddMember("score", data.score, allocator);
    obj.AddMember("name", rapidjson::Value(data.name.c_str(), data.name.size(), allocator), allocator);

    rapidjson::Value point_array(rapidjson::kArrayType);
    for (auto& point : data.contour)
    {
        rapidjson::Value point_value(rapidjson::kObjectType);
        point_value.AddMember("x", point.x, allocator);
        point_value.AddMember("y", point.y, allocator);
        point_array.PushBack(point_value, allocator);
    }
    obj.AddMember("contour", point_array, allocator);
    return obj;
}
