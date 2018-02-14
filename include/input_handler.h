#pragma once

#include <string>

#include "block_builder.h"
#include "command_factory.h"

class input_handler
{
    public:
        virtual ~input_handler() noexcept { }
        virtual bool next(const std::string& s) = 0;
        virtual void flush() = 0;
};

class command_block_handler : public input_handler
{
    private:
        command_factory* m_cf = nullptr;
        block_builder* m_b = nullptr;

    public:
        command_block_handler(command_factory* cf, block_builder* b);
        bool next(const std::string& s) override;
        void flush() override;
};
