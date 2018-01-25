#pragma once

#include <memory>
#include <string>

#include "document.h"
#include "shape_stream.h"

class controller
{
    private:
        std::unique_ptr<document> doc;

    private:
        template <typename T>
        std::unique_ptr<T> stream_from_uri(const std::string&)
        {
            return nullptr;
        }

    public:
        void create_document()
        {
            doc.reset(new document);
        }

        void create_shape(const std::string& type, const point& p)
        {
            auto s = shape_creator::make_shape(type);
            s->move_to(p);
            doc->add_shape(s);
        }

        void remove_shape(const point& p)
        {
            auto s = doc->shape_from_point(p);
            doc->remove_shape(s);
        }

        void import_from(const std::string& uri)
        {
            auto in(stream_from_uri<IShapeStreamIn>(uri));
            doc->import_from(*in);
        }

        void export_to(const std::string& uri)
        {
            auto out(stream_from_uri<IShapeStreamOut>(uri));
            doc->export_to(*out);
        }
};
