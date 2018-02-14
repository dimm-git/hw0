#include "command_block.h"

void command_block::add(command* cmd)
{
    m_commands.emplace_back(cmd);
}

void command_block::clear()
{
    m_commands.clear();
}

command_block::const_iterator command_block::begin() const
{
    return m_commands.begin();
}

command_block::const_iterator command_block::end() const
{
    return m_commands.end();
}
