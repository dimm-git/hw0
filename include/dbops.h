#pragma once

#include "operation.h"

class op_insert : public operation
{
    private:
        std::string m_table;
        record m_record;

    public:
        void initialize(const operation_args& args) override;
        table_name_list affected() const override;
        void apply(operation_result& res, table_list& list) override;
};

class op_truncate : public operation
{
    private:
        std::string m_table;

    public:
        void initialize(const operation_args& args) override;
        table_name_list affected() const override;
        void apply(operation_result& res, table_list& list) override;
};

class result_proxy
{
    private:
        operation_result& m_res;
        table& m_second;

    public:
        result_proxy(operation_result& res, table& second);
        result_proxy& operator=(const record_type& rec);
};

class isect_iterator
{
    private:
        operation_result& m_res;
        table& m_second;

    public:
        isect_iterator(operation_result& res, table& second);
        isect_iterator& operator++();
        isect_iterator operator++(int);
        result_proxy operator*();
};

class op_intersect : public operation
{
    private:
        std::string m_tab0;
        std::string m_tab1;

    public:
        void initialize(const operation_args& args) override;
        table_name_list affected() const override;
        void apply(operation_result& res, table_list& list) override;        
};
