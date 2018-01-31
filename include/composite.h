#pragma once

#include <algorithm>
#include <vector>

template <typename T>
struct composite
{
    virtual std::size_t size() const = 0;
    virtual std::size_t length() const = 0;

    virtual composite* at(std::size_t index) = 0;
    virtual composite* off(std::size_t off) = 0;
    virtual std::size_t index() = 0;
    virtual composite* parent() = 0;

    virtual T& value() = 0;
    virtual void set_val(const T& v) = 0;

    virtual void add(composite<T>* v) = 0;
    virtual void remove(composite<T>* v) = 0;

    virtual ~composite() throw() { }
};

template <typename T, T DefVal>
class mvalue : public composite<T>
{
    private:
        typedef composite<T> parent_type;
        T v = DefVal;
        std::size_t ind = (std::size_t)-1;

    public:
        mvalue(const T& val, std::size_t index) : v(val), ind(index)
        {
        }

        std::size_t size() const override
        {
            return 1;
        }

        std::size_t length() const override
        {
            return 1;
        }

        composite<T>* at(std::size_t) override
        {
            throw std::runtime_error("mvalue::at called");
        }

        composite<T>* off(std::size_t) override
        {
            throw std::runtime_error("mvalue::off called");
        }

        T& value() override
        {
            return v;
        }

        void set_val(const T& val) override
        {
            v = val;
        }

        std::size_t index() override
        {
            return ind;
        }

        void add(composite<T>*) override
        {
        }

        void remove(composite<T>*) override
        {
        }

        composite<T>* parent() override
        {
            return nullptr;
        }
};

template <typename T, T DefVal>
class subdim : public composite<T>
{
    private:
        typedef composite<T> parent_type;
        std::vector<parent_type*> items;
        parent_type* par = nullptr;
        std::size_t ind = (std::size_t)-1;

    public:
        subdim(parent_type* p, std::size_t index) : par(p), ind(index)
        {
        }

        ~subdim() throw()
        {
            auto f = [ ](const decltype(items[0])& v) { delete v; };
            std::for_each(items.begin(), items.end(), f);
        }

        void add(composite<T>* val) override
        {
            items.push_back(val);
        }

        void remove(composite<T>* v) override
        {
            items.erase(std::remove(items.begin(), items.end(), v), items.end());
            delete v;
        }

        std::size_t size() const override
        {
            std::size_t sum = 0;
            auto f = [ & sum ](const decltype(items[0])& v) { sum += v->size(); };
            std::for_each(items.begin(), items.end(), f);
            return sum;
        }

        std::size_t length() const override
        {
            return items.size();
        }

        composite<T>* at(std::size_t index) override
        {
            for (auto i : items)
            {
                if (i->index() == index)
                    return i;
            }

            return nullptr;
        }

        composite<T>* off(std::size_t off) override
        {
            return off < items.size() ? items[off] : nullptr;
        }

        T& value() override
        {
            throw std::runtime_error("dim::value called");
        }

        void set_val(const T&) override
        {
            throw std::runtime_error("dim::st_val called");
        }

        std::size_t index() override
        {
            return ind;
        }

        composite<T>* parent() override
        {
            return par;
        }
};
