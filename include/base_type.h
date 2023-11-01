#pragma once
#include <vector>
#include <string>

typedef struct Point
{
    int x;
    int y;
    float score;
}Point;

typedef struct Bbox
{
    int x1;
    int y1;
    int x2;
    int y2;
    int id;
    float score;
} Bbox;

typedef struct ClassInfo
{
    int id;
    float score;
    std::string name;
} ClassInfo;

typedef struct DetectInfo
{
    int id;
    float score;
    std::string name;
    Bbox bbox;
}DetectInfo;

typedef struct SegInfo
{
    int id;
    float score;
    std::string name;
    std::vector<Point> contour;
}SegInfo;