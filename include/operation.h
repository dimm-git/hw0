#pragma once

#include <list>
#include <string>
#include <vector>

#include "table.h"

using table_name_list = std::list<std::string>;
using table_list = std::vector<table*>;
using operation_args = std::list<std::string>;
using operation_result = std::list<result_record>;

class operation
{
    public:
        virtual ~operation() noexcept { }
        virtual void initialize(const operation_args& args) = 0;
        virtual table_name_list affected() const = 0;
        virtual void apply(operation_result& res, table_list& list) = 0;

        virtual bool modification() const noexcept = 0;
};
