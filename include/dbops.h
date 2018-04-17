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
        void apply(table_list& list) override;
};

class op_truncate : public operation
{
    private:
        std::string m_table;

    public:
        void initialize(const operation_args& args) override;
        table_name_list affected() const override;
        void apply(table_list& list) override;
};
