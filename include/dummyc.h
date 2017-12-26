#pragma once

#include <memory>

namespace dummy_private
{
    template <typename T>
    struct dummy_node
    {
        T val;
        dummy_node* next = nullptr;
    };
}

template <typename T>
class dummy_const_iterator
{
    public:
        typedef dummy_private::dummy_node<T> node_type;
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef std::forward_iterator_tag iterator_category;

        typedef const T* pointer;
        typedef const T& reference;

        dummy_const_iterator() noexcept
        {
        }

        explicit dummy_const_iterator(node_type* n) noexcept
            : m_n(n)
        {
        }

        dummy_const_iterator(const dummy_const_iterator& i) noexcept
            : m_n(i.m_n)
        {
        }

        reference operator*() const noexcept
        {
            return m_n->val;
        }

        pointer operator->() const noexcept
        {
            return &m_n->val;
        }

        dummy_const_iterator& operator++() noexcept
        {
            if (m_n != nullptr)
                m_n = m_n->next;
            return *this;
        }

        dummy_const_iterator& operator++(int) noexcept
        {
            dummy_const_iterator tmp(*this);
            tmp++;
            return tmp;
        }

        bool operator==(const dummy_const_iterator& o)
        {
            return o.m_n == m_n;
        }

        bool operator!=(const dummy_const_iterator& o)
        {
            return o.m_n != m_n;
        }

    private:
        node_type* m_n;
};

template <typename T, typename A = std::allocator<T> >
class dummy_container
{
    private:
        typedef dummy_private::dummy_node<T> node;
        typedef typename A::template rebind<node>::other node_allocator;
        typedef typename A::template rebind<T>::other value_allocator;

        value_allocator m_va = value_allocator();
        node_allocator m_na = node_allocator(m_va);

        void deleter(node* n)
        {
            while (n != nullptr)
            {
                node* next = n->next;
                m_va.destroy(&n->val);
                m_na.destroy(n);
                m_na.deallocate(n, 1);
                n = next;
            }
        }

        node* m_head = nullptr;
        node* m_tail = nullptr;

    public:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef dummy_const_iterator<T> const_iterator;

        dummy_container(const dummy_container&) = delete;
        dummy_container(dummy_container&& o)
        {
            m_head = o.m_head;
            m_tail = o.m_tail;
            o.m_head = o.m_tail = nullptr;
        }

        dummy_container& operator=(const dummy_container&) = delete;
        dummy_container& operator=(dummy_container&&) = delete;

        dummy_container()
        {
        }

        dummy_container(std::initializer_list<value_type> l, const value_allocator& alloc = value_allocator()) :
            m_va(alloc),
            m_na(node_allocator(alloc))
        {
            std::for_each(l.begin(), l.end(), [ & ](value_type v) { insert(v); });
        }

        void insert(const_reference v)
        {
            do_insert(v);
        }

        void insert(T&& v)
        {
            do_insert(std::move(v));
        }
       
        template<typename... Args>
        void emplace(Args&&... args)
        {
            do_insert(std::forward<Args>(args)...);
        }

        ~dummy_container() throw()
        {
            if (m_head != nullptr)
                deleter(m_head);
            m_head = m_tail = nullptr;
        }

        const_iterator cbegin() const
        {
            return const_iterator(m_head);
        }

        const_iterator cend() const
        {
            return const_iterator(nullptr);
        }

    private:
        template <typename... Args>
        void do_insert(Args&&... args)
        {
            std::unique_ptr<node> n(m_na.allocate(1));
            m_va.construct(&n->val, std::forward<Args>(args)...);
            if (m_head == nullptr)
            {
                m_head = m_tail = n.release();
                m_head->next = nullptr;
            }
            else
            {
                m_tail->next = n.release();
                m_tail = m_tail->next;
                m_tail->next = nullptr;
            }
        }
};
