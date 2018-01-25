#pragma once

#include <algorithm>
#include <list>

#include "shape.h"

class document
{
    private:
        typedef std::list<IShape*> shapes;
        shapes l;

    public:
        void add_shape(IShape* s) { l.push_back(s); }
        void remove_shape(IShape* s) { l.remove(s); }

        IShape* shape_from_point(const point&)
        {
            return nullptr;
        }

        void import_from(IShapeStreamIn& s)
        {
            while (s.next() == true)
            {
                IShape* sh = shape_creator::make_shape(s.current_shape());
                sh->import_from(s);
            }
        }

        void export_to(IShapeStreamOut& s)
        {
            auto f = [ &s ](IShape* sh) { sh->export_to(s); };
            std::for_each(l.begin(), l.end(), f);
        }
};
