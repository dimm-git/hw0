#pragma once

#include <string>

class command
{
    public:
        virtual ~command() noexcept { }
        virtual const std::string& name() const noexcept = 0;
};

class input_command : public command
{
    private:
        std::string m_cmd;

    public:
        input_command(const std::string& cmd);
        const std::string& name() const noexcept override;
};
