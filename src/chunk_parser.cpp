#include <algorithm>
#include <iterator>

#include "chunk_parser.h"

namespace
{

class the_ctype : public std::ctype<char>
{
    private:
        mask the_table[table_size];

    public:
        the_ctype(size_t refs = 0) : std::ctype<char>(&the_table[0], false, refs)
        {
            std::copy_n(classic_table(), table_size, the_table);
            auto pred = [ ](decltype(the_table[0]) val) { return (val & ctype_base::space) == ctype_base::space; };
            for (std::size_t i = 0; i < table_size; i++)
            {
                if (pred(the_table[i]) == true)
                    the_table[i] &= ~ctype_base::space;
            }
            the_table['\n'] |= (mask)space;
        }
};

struct rit
{
    typedef const char* value_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    typedef const char* pointer;
    typedef const char*& reference;

    const char* ptr = 0;

    explicit rit(const char* i) : ptr(i) { }

    rit& operator++()
    {
        --ptr;
        return *this;
    }

    operator const char*()
    {
        return ptr;
    }
};

}

bulk_adapter::bulk_adapter(input_handler* h) : m_h(h)
{
    std::locale x(std::locale::classic(), new the_ctype);
    buf.imbue(x);
}

void bulk_adapter::process(const char* data, std::size_t size)
{
    if (size == 0)
        return;

    auto dbeg = rit(data + size - 1);
    auto dend = rit(data - 1);

    auto pos = std::find(dbeg, dend, '\n');

    if (dend != pos)
    {
        std::size_t lp = pos - data;
        buf.write(data, lp);
        std::istream_iterator<std::string> ss{buf};
        std::istream_iterator<std::string> se;
        auto h = m_h;
        auto proc = [ h ](std::string line)
        {
            h->next(line);
        };
        std::for_each(ss, se, proc);
        data = pos;
        size = dbeg - pos;
    }
    buf.clear();
    if (size != 0)
        buf.write(data, size);
}

void bulk_adapter::done()
{
}
