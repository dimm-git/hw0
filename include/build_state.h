#pragma once

#include <array>

#include "command.h"

enum block_state
{
    bsAccept,
    bsReject,
    bsError,
    bsFinish
};

class build_state
{
    public:
        virtual ~build_state() noexcept { }
        virtual void reset() = 0;

        virtual block_state push() = 0;
        virtual block_state check(command* cmd) = 0;
        virtual block_state pop() = 0;
        virtual block_state flush() = 0;
};

class build_state_plain : public build_state
{
    private:
        int m_max_size;
        int m_cur_size = 0;

    public:
        build_state_plain(int size);
        void reset() override;

        block_state push() override;
        block_state check(command* cmd) override;
        block_state pop() override;
        block_state flush() override;
};

class build_state_nested : public build_state
{
    private:
        int m_level = 0;

    public:
        void reset() override;

        block_state push() override;
        block_state check(command* cmd) override;
        block_state pop() override;
        block_state flush() override;
};

class build_state_mixed : public build_state
{
    private:
        build_state* m_plain = nullptr;
        build_state* m_rec = nullptr;
        build_state* m_cur = nullptr;

    public:
        build_state_mixed(build_state* plain, build_state* rec);
        void reset() override;

        block_state push() override;
        block_state check(command* cmd) override;
        block_state pop() override;
        block_state flush() override;
};
