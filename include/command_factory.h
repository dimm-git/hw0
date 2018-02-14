#pragma once

#include <string>

#include "command.h"

class command_factory
{
    public:
        virtual command* make(const std::string& cmd) = 0;
};

class input_command_factory : public command_factory
{
    public:
        command* make(const std::string& cmd) override
        {
            return new input_command(cmd);
        }
};
