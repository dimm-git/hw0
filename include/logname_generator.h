#pragma once

#include <ctime>
#include <string>

class logname_generator
{
    public:
        virtual ~logname_generator() noexcept { }
        virtual void update_timestamp() = 0;
        virtual std::string name() = 0;
};

class logname_generator_prod : public logname_generator
{
    private:
        std::time_t m_ts = 0;

    public:
        void update_timestamp() override;
        std::string name() override;
};

class logname_generator_debug : public logname_generator
{
    private:
        struct timeval m_tv = { 0, 0 };

    public:
        void update_timestamp() override;
        std::string name() override;
};
