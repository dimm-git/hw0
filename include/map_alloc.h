#pragma once

#include <list>
#include <memory>

namespace alloc_private
{
template <typename T>
struct alloc_node
{
    public:
    alloc_node(T* s = nullptr, std::size_t c = 0) : start(s), count(c)
    {
    }

    void on_dealloc(std::size_t n)
    {
        n = std::min(n, inuse);
        inuse -= n;
        if (inuse == 0)
            curpos = 0;
    }

    T* on_alloc(std::size_t n)
    {
        if (has_space(n) == false)
            throw std::runtime_error("on_alloc");
        T* ret = &start[curpos];
        inuse += n;
        curpos += n;
        return ret;
    }

    bool has_space(std::size_t n) const
    {
        return n <= (count - curpos);
    }

    const alloc_node* get() const { return this; }
    alloc_node* get() { return this; }

    T* start = nullptr;
    std::size_t count = 0;
    std::size_t inuse = 0;
    std::size_t curpos = 0;
    alloc_node* next = nullptr;
};

template <typename T>
class alloc_iterator
{
    public:
        typedef alloc_node<T> node_type;
        typedef node_type value_type;
        typedef std::ptrdiff_t difference_type;
        typedef std::forward_iterator_tag iterator_category;

        typedef node_type* pointer;
        typedef node_type& reference;

        alloc_iterator() noexcept
        {
        }

        explicit alloc_iterator(node_type* n) noexcept
            : m_n(n)
        {
        }

        alloc_iterator(const alloc_iterator& i) noexcept
            : m_n(i.m_n)
        {
        }

        reference operator*() const noexcept
        {
            return *m_n;
        }

        pointer operator->() const noexcept
        {
            return m_n;
        }

        alloc_iterator& operator++() noexcept
        {
            if (m_n != nullptr)
                m_n = m_n->next;
            return *this;
        }

        alloc_iterator& operator++(int) noexcept
        {
            alloc_iterator tmp(*this);
            tmp++;
            return tmp;
        }

        bool operator==(const alloc_iterator& o)
        {
            return o.m_n == m_n;
        }

        bool operator!=(const alloc_iterator& o)
        {
            return o.m_n != m_n;
        }

    private:
        node_type* m_n;
};

template <typename  T, std::size_t default_step = 10>
class map_allocator
{
    private:
        // MMAP_MALLOC_THRESHOLD + dynamic mmap threshold
        // std::unique_ptr<T, decltype(std::free)*> m_base = { nullptr, &std::free };
        typedef alloc_node<T> node;
        std::unique_ptr<node, void (*)(node*)> m_pool = { nullptr,
            [](node* n)
            {
                while (n != nullptr)
                {
                    auto next = n->next;
                    delete[] reinterpret_cast<char*>(n->start);
                    delete n;
                    n = next;
                }
            }
        };
        node* m_tail = nullptr;
        std::size_t m_step = 10;

    public:
        using value_type = T;

        map_allocator() : m_step(default_step)
        {
        }

        map_allocator(map_allocator&&) = default;

        map_allocator(const map_allocator& a) : m_step(a.m_step)
        {
        }

        template <typename U>
        map_allocator(const map_allocator<U>& a) : m_step(a.step())
        {
        }

        map_allocator& operator=(const map_allocator&) = delete;
        map_allocator& operator=(map_allocator&&) = delete;

        ~map_allocator() throw() = default;

        T* allocate(std::size_t n = step())
        {
            node* c = find_node(n);
            if (c == nullptr)
                throw std::bad_alloc();
            return c->on_alloc(n);
        }

        void deallocate(T* p, std::size_t n)
        {
            auto l = [ p ](const node& nd)
            {
                if (nd.start > p)
                    return false;
                auto lp = nd.start + nd.count;
                return p < lp;
            };
            auto i = std::find_if(begin(), end(), l);
            if (i == end())
                throw std::runtime_error("block not found");
            node* nd = i->get();
            nd->on_dealloc(n);
        }

        template <class U, class... Args>
        void construct(U* p, Args&&... args)
        {
            new((void *)p) U(std::forward<Args>(args)...);
        }

        void destroy(T* p)
        {
            p->~T();
        }

        template <typename U>
        struct rebind
        {
            typedef map_allocator<U> other;
        };

        std::size_t step() const
        {
            return m_step;
        }

    private:
        typedef alloc_iterator<T> node_iterator;
        node_iterator begin() { return node_iterator(m_pool.get()); }
        node_iterator end() { return node_iterator(nullptr); }

        auto find_node(std::size_t n)
        {
            auto l = [ n ](const node& nd) { return nd.has_space(n); };
            auto i = std::find_if(begin(), end(), l);
            if (i == end())
                return try_alloc(n);

            return i->get();
        }

        auto try_alloc(std::size_t n)
        {
            n = std::max(n, m_step);
            std::size_t size = m_step * sizeof(T);
            // MMAP_MALLOC_THRESHOLD + dynamic mmap threshold
            // auto ptr = reinterpret_cast<T*>(std::malloc(size));
            auto ptr = reinterpret_cast<T*>(new char[size]);
            auto nd = new node(ptr, n);
            if (m_tail != nullptr)
            {
                m_tail->next = nd;
                m_tail = nd;
            }
            else
            {
                m_pool.reset(nd);
                m_tail = nd;
            }
            return nd;
        }
};
}

template <std::size_t default_step = 10>
struct custom_allocator
{
    template <typename T>
    using allocator = alloc_private::map_allocator<T, default_step>;
};
