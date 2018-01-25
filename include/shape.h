#pragma once

#include "point.h"
#include "shape_stream.h"

struct IShape
{
    virtual const rect& get_rect() = 0;
    virtual void move_to(const point& p) = 0;

    virtual void import_from(IShapeStreamIn& s) = 0;
    virtual void export_to(IShapeStreamOut& s) = 0;
};

struct shape_creator
{
    static IShape* make_shape(const std::string&) { return nullptr; }
};
