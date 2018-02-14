#include "command.h"

input_command::input_command(const std::string& cmd) : m_cmd(cmd)
{
}

const std::string& input_command::name() const noexcept
{
    return m_cmd;
}
