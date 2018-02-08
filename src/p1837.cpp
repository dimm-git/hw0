#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct person
{
    bool inproc;
    int id;
    int rank;
    std::vector<int> teammates;
    person(int i = -1) : inproc(false), id(i), rank(-1)
    {
    }
};

person* ensure(std::map<std::string, person*>& i, std::vector<person>& p, const std::string& name, int& id)
{
    auto& n = i[name];
    if (n == nullptr)
    {
        p.push_back(person(id++));
        n = &p[p.size() - 1];
        std::cout << name << " " << n->id << std::endl;
    }
    return n;
}

void add(person* n0, person* n1, person* n2)
{
    n0->teammates.push_back(n1->id);
    n0->teammates.push_back(n2->id);
}

void mix(person* n0, person* n1, person* n2)
{
    add(n0, n1, n2);
    add(n1, n0, n2);
    add(n2, n0, n1);
}

void mark(person* p, std::map<std::string, person*>& i, std::vector<person>& a)
{
    std::cout << "mark for " << p->id << std::endl;
    for (std::size_t i = 0; i < p->teammates.size(); i++)
    {
        person& c = a[p->teammates[i]];
        if (c.rank == -1)
            c.rank = p->rank + 1;
        std::cout << "  rank for " << p->teammates[i] << " " << c.id << " " << c.rank << std::endl;
    }
    for (std::size_t j = 0; j< p->teammates.size(); j++)
    {
        int id = p->teammates[j];
        person& c = a[id];
        std::cout << c.id << " " << c.rank << " " << c.inproc << std::endl;
        if (c.inproc == false)
        {
            c.inproc = true;
            mark(&c, i, a);
        }
    }
}

void solve(std::map<std::string, person*>& i, std::vector<person>& p)
{
    std::vector<int> to(300);
    auto it = i.find("Isenbaev");
    if (it == i.end())
        return;
    person* is = it->second;
    is->rank = 0;
    is->inproc = true;
    mark(is, i, p);
}
 
int main()
{
    std::vector<person> persons(300);
    std::map<std::string, person*> index;
    int id = 0;

    std::string name;
    std::size_t num = 0;

    std::cin >> num;

    while (num > 0)
    {
        std::cin >> name;
        auto n0 = ensure(index, persons, name, id);
        std::cin >> name;
        auto n1 = ensure(index, persons, name, id);
        std::cin >> name;
        auto n2 = ensure(index, persons, name, id);
        mix(n0, n1, n2);
        --num;
    }
    solve(index, persons);
    auto p = [ ](decltype(index)::value_type& p)
    {
        std::cout << p.first << " ";
        if (p.second->rank == -1)
            std::cout << "undefined";
        else
            std::cout << p.second->rank;
        std::cout << std::endl;
    };
    std::for_each(index.begin(), index.end(), p);

    return 0;
}