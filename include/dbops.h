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
        bool modification() const noexcept override
        {
            return true;
        }
};

class op_truncate : public operation
{
    private:
        std::string m_table;

    public:
        void initialize(const operation_args& args) override;
        table_name_list affected() const override;
        void apply(operation_result& res, table_list& list) override;
        bool modification() const noexcept override
        {
            return true;
        }
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

        bool modification() const noexcept override
        {
            return false;
        }

};

class op_sym_diff : public operation
{
    private:
        std::string m_tab0;
        std::string m_tab1;

    public:
        void initialize(const operation_args& args) override;
        table_name_list affected() const override;
        void apply(operation_result& res, table_list& list) override;
        bool modification() const noexcept override
        {
            return false;
        }
};
