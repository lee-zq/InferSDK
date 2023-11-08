#pragma once

#include "com/utils/dataset_class_id_map.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// COCO dataset class id map
std::unordered_map<int, std::string> coco_class_id_map = {{0, "person"},         {1, "bicycle"},       {2, "car"},
                                                          {3, "motorcycle"},     {4, "airplane"},      {5, "bus"},
                                                          {6, "train"},          {7, "truck"},         {8, "boat"},
                                                          {9, "traffic light"},  {10, "fire hydrant"}, {11, "stop sign"},
                                                          {12, "parking meter"}, {13, "bench"},        {14, "bird"},
                                                          {15, "cat"},           {16, "dog"},          {17, "horse"},
                                                          {18, "sheep"},         {19, "cow"},          {20, "elephant"},
                                                          {21, "bear"},          {22, "zebra"},        {23, "giraffe"},
                                                          {24, "backpack"},      {25, "umbrella"},     {26, "handbag"},
                                                          {27, "tie"},           {28, "suitcase"},     {29, "frisbee"},
                                                          {30, "skis"},          {31, "snowboard"},    {32, "sports ball"},
                                                          {33, "kite"},          {34, "baseball bat"}, {35, "baseball glove"},
                                                          {36, "skateboard"},    {37, "surfboard"},    {38, "tennis racket"},
                                                          {39, "bottle"},        {40, "wine glass"},   {41, "cup"},
                                                          {42, "fork"},          {43, "knife"},        {44, "spoon"},
                                                          {45, "bowl"},          {46, "banana"},       {47, "apple"},
                                                          {48, "sandwich"},      {49, "orange"},       {50, "broccoli"},
                                                          {51, "carrot"},        {52, "hot dog"},      {53, "pizza"},
                                                          {54, "donut"},         {55, "cake"},         {56, "chair"},
                                                          {57, "couch"},         {58, "potted plant"}, {59, "bed"},
                                                          {60, "dining table"},  {61, "toilet"},       {62, "tv"},
                                                          {63, "laptop"},        {64, "mouse"},        {65, "remote"},
                                                          {66, "keyboard"},      {67, "cell phone"},   {68, "microwave"},
                                                          {69, "oven"},          {70, "toaster"},      {71, "sink"},
                                                          {72, "refrigerator"},  {73, "book"},         {74, "clock"},
                                                          {75, "vase"},          {76, "scissors"},     {77, "teddy bear"},
                                                          {78, "hair drier"},    {79, "toothbrush"}};
// VOC dataset class id map
std::unordered_map<int, std::string> static voc_class_id_map = {
    {0, "background"}, {1, "aeroplane"}, {2, "bicycle"},    {3, "bird"},    {4, "boat"},         {5, "bottle"},
    {6, "bus"},        {7, "car"},       {8, "cat"},        {9, "chair"},   {10, "cow"},         {11, "diningtable"},
    {12, "dog"},       {13, "horse"},    {14, "motorbike"}, {15, "person"}, {16, "pottedplant"}, {17, "sheep"},
    {18, "sofa"},      {19, "train"},    {20, "tvmonitor"}};
std::string get_coco_class_name(int class_id)
{
    for (auto iter = coco_class_id_map.begin(); iter != coco_class_id_map.end(); iter++)
    {
        if (iter->first == class_id)
        {
            return iter->second;
        }
    }
    return "Unknown";
}

std::string get_voc_class_name(int class_id)
{
    for (auto iter = voc_class_id_map.begin(); iter != voc_class_id_map.end(); iter++)
    {
        if (iter->first == class_id)
        {
            return iter->second;
        }
    }
    return "Unknown";
}