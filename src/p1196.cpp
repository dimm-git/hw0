#include <algorithm>
#include <iostream>
#include <set>
 
int main()
{
    typedef std::set<int> Set;
    std::size_t num = 0;
    Set data;
    std::cin >> num;
    while (num > 0)
    {
        int date;
        std::cin >> date;
        data.insert(date);
        --num;
    }
    std::cin >> num;
    std::size_t count = 0;
    while (num > 0)
    {
        int date;
        std::cin >> date;
        --num;
        if (data.find(date) != data.end())
            count++;
    }

    std::cout << count << std::endl;

    return 0;
}