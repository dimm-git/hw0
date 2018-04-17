#pragma once

#include <memory>
#include <set>
#include <string>

#include "record.h"

struct record_less
{
    bool operator()(const record& l, const record& r) const;
};

class table
{
    public:
        // static bool record_less(const record& l, const record& r);
        using records = std::set<record, record_less>;
        using iterator = records::iterator;
        using const_iterator = records::const_iterator;

    private:
        std::string m_name;
        records m_records;

    public:
        table(std::string name);

        void insert(record&& rec);
        void truncate();
        const std::string get_name() const;

        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;
};
