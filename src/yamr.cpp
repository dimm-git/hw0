#include <iostream>

#include "mapping.h"
#include "shuffle.h"
#include "reduce.h"

void run(const std::string& src, int mnum, int rnum);

int main(int argc, char* argv[])
{
    int ret = 1;
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << "<src> <mnum> <rnum>" << std::endl;
        return 1;
    }

    try
    {
        run(argv[1], std::atoi(argv[2]), std::atoi(argv[3]));
        ret = 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "err: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "err: unknown exception" << std::endl;
    }
    return ret;
}

void run(const std::string& src, int mnum, int rnum)
{
    std::vector<results> res;
    {
        maps s;
        {
            maps m;
            make_map(m, src, mnum);
            make_shuffle(s, m, rnum);
        }
        make_reduce(res, s);
#if 0
        for (auto& i : s)
        {
            for (auto& j: i)
                std::cout << j << std::endl;
            std::cout << std::endl;
        }
#endif
    }

}
