#include "input_handler.h"

command_block_handler::command_block_handler(command_factory* cf, block_builder* b) : m_cf(cf), m_b(b)
{
}
 
bool command_block_handler::next(const std::string& s)
{
    bool res = false;
    if (s == "}")
        res = m_b->command_pop();
    else if (s == "{")
        res = m_b->command_push();
    else
    {
        std::unique_ptr<command> cmd(m_cf->make(s));
        res = m_b->command_add(cmd.get());
#if 0
        if (res == true)
            cmd.release();
#endif
    }
    return res;
}

void command_block_handler::flush()
{
    m_b->flush();
}
