#pragma once

#include <fstream>

#include "logname_generator.h"

class app_mode
{
    public:
        virtual ~app_mode() noexcept { }
        virtual std::istream& get_istream() = 0;
        virtual int get_block_size() = 0;
        virtual std::string usage(const char* app) = 0;
        virtual void parse(int argc, const char* argv[]) = 0;
        virtual logname_generator& get_generator() = 0;
        virtual std::size_t get_queue_length() const noexcept = 0;
};

class app_mode_prod : public app_mode
{
    private:
        int m_depth = 0;
        logname_generator_ms m_gen;

    public:
        std::istream& get_istream() override;
        int get_block_size() override;
        void parse(int argc, const char* argv[]) override;
        std::string usage(const char* app) override;
        logname_generator& get_generator() override;
        std::size_t get_queue_length() const noexcept override;
};

class app_mode_debug : public app_mode
{
    private:
        int m_depth = 0;
        logname_generator_ms m_gen;
        std::ifstream m_strm;
        std::size_t m_ql = 1;

    public:
        std::istream& get_istream() override;
        int get_block_size() override;
        void parse(int argc, const char* argv[]) override;
        std::string usage(const char* app) override;
        logname_generator& get_generator() override;
        std::size_t get_queue_length() const noexcept override;
};
