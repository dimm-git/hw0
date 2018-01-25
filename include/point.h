#pragma once

#include <stdint.h>

struct point
{
    int32_t x;
    int32_t y;
    point(int32_t _x, int32_t _y) : x(_x), y(_y) {}
};

struct rect
{
    point ltop;
    point rbot;
};
