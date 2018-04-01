#pragma once

#include <list>
#include <memory>

#include "command.h"

// TODO: 
// 1. constructor + virtual destructor?
// 2. iterator proxy (unique_ptr?)
class command_block
{
    public:
        using commands = std::list<std::string >;
        using iterator = commands::iterator;
        using const_iterator = commands::const_iterator;

        void add(command* cmd);
        void clear();

        const_iterator begin() const;
        const_iterator end() const;

    private:
         commands m_commands;
};
