/// \file

#include <iostream>

#include "controller.h"
#include "document.h"
#include "shape.h"
#include "shape_stream.h"

static void on_new_document(controller& c);
static void on_import_doc(controller& c, const std::string& name);
static void on_export_doc(controller& c, const std::string& name);
static void on_new_shape(controller& c, const std::string& type, const point& p);
static void on_remove_shape(controller& c, const point& p);

int main(int, char**)
{
    try
    {
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

void on_new_document(controller& c)
{
    c.create_document();
}

void on_import_doc(controller& c, const std::string& name)
{
    c.import_from(name);
}

void on_export_doc(controller& c, const std::string& name)
{
    c.export_to(name);
}

void on_new_shape(controller& c, const std::string& type, const point& p)
{
    c.create_shape(type, p);
}

void on_remove_shape(controller& c, const point& p)
{
    c.remove_shape(p);
}
