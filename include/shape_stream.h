#pragma once

#include <string>

struct IShape;

struct IShapeStreamIn
{
    virtual bool next() = 0;
    virtual std::string current_shape() = 0;
    virtual std::string read_param(const std::string& name) = 0;
    virtual void close() = 0;
};

struct IShapeStreamOut
{
    virtual void begin_shape(const std::string& name) = 0;
    virtual void write_param(const std::string& name, const std::string& value) = 0;
    virtual void end_shape() = 0;
    virtual void close() = 0;
};
