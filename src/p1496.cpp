#include <algorithm>
#include <iostream>
#include <map>
 
int main()
{
    typedef std::map<std::string, int> Map;
    std::size_t num = 0;
    Map data;
    std::cin >> num;
    std::string line;
    std::getline(std::cin, line);
    while (num > 0)
    {
        std::getline(std::cin, line);
        auto& s = data[line];
        s++;
        --num;
    }
    auto f = [ ](const Map::value_type& i) { if (i.second > 1) std::cout << i.first << std::endl; };
    std::for_each(data.begin(), data.end(), f);
    
    return 0;
}