#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <type_traits>
#include <utility>

#include "block_shared.h"

class evil_queue
{
    public:
        using block_pair = std::pair<block_shared, std::string>;

    private:
        std::mutex m_lock;
        std::condition_variable m_cond;
        std::queue<block_pair> m_blocks;
        bool m_stop = false;

    public:
        block_pair pop();
        void push(block_shared block, const std::string& ts);
        void stop();
};
