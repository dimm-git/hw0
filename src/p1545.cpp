#include <algorithm>
#include <iostream>
#include <map>
#include <set>
 
int main()
{
    std::size_t num = 0;
    std::map<char, std::set<char> > data;
    std::cin >> num;
    std::string line;
    std::getline(std::cin, line);
    while (num > 0)
    {
        std::getline(std::cin, line);
        auto& s = data[line[0]];
        s.insert(line[1]);
        --num;
    }
    char x = 0;
    std::cin >> x;
    auto i = data.find(x);
    if (i != data.end())
    {
        char buf[] = { x, 0, 0 };
        auto& s = i->second;
        auto f = [ &buf ](char y) { buf[1] = y; std::cout << buf << std::endl; };
        std::for_each(s.begin(), s.end(), f);
    }
    
    return 0;
}