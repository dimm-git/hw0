
#pragma once

#include <ostream>

#include "command.h"
#include "command_block.h"

class command_printer
{
    public:
        virtual ~command_printer() noexcept { }

        virtual void print(std::ostream& os, const command& cmd) = 0;
};

class block_printer
{
    public:
        virtual std::size_t print(std::ostream& s, const command_block& block) = 0;
};

class command_printer_name : public command_printer
{
    public:
        void print(std::ostream& strm, const command& cmd) override;
};

class block_printer_bulk : public block_printer
{
    private:
        command_printer& m_cmdp;

    public:
        block_printer_bulk(command_printer& p);
        std::size_t print(std::ostream& s, const command_block& block) override;
};

class onion_block_printer : public block_printer
{
    private:
        block_printer* sibling = nullptr;
        std::size_t size = 0;
        std::size_t count = 0;

    public:
        onion_block_printer(block_printer* printer, std::size_t fbsize, std::size_t fbcount);
        std::size_t print(std::ostream& s, const command_block& block) override;
};
