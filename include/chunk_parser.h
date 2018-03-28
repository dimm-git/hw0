#pragma once

#include <memory>
#include <sstream>

#include "input_handler.h"

class bulk_adapter
{
    private:
        input_handler* m_h;
        std::stringstream buf;

    public:
        bulk_adapter(input_handler* h);

        void process(const char* data, std::size_t size);
        void done();
};
