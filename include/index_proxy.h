#pragma once

#include "composite.h"

template <typename T>
struct index_proxy_base
{
    composite<T>* val = nullptr;
    std::size_t ind = -1;
    index_proxy_base(composite<T>* c, std::size_t index) : val(c), ind(index)
    {
    }

    virtual composite<T>* on_add()
    {
        return val;
    }

    virtual void on_remove(composite<T>*)
    {
    }
};

template <typename T, T DefVal, std::size_t Dim>
class index_proxy : public index_proxy_base<T>
{
    private:
        index_proxy_base<T>& parent;

    public:
        index_proxy(index_proxy_base<T>& p, std::size_t index = -1, composite<T>* c = nullptr) : index_proxy_base<T>(c, index), parent(p)
        {
        }

        ~index_proxy() throw()
        {
            if ((this->val != nullptr) && (this->val->length() == 0))
            {
                composite<T>* parent = this->val->parent();
                parent->remove(this->val);
            }
        }

        index_proxy<T, DefVal, Dim - 1> operator[](std::size_t i)
        {
            if (this->val != nullptr)
                return index_proxy<T, DefVal, Dim - 1>(*this, i, this->val->at(i));
            return index_proxy<T, DefVal, Dim - 1>(*this, i);
        }

        bool operator==(const T& v) const
        {
            return false;
        }

        composite<T>* on_add() override
        {
            if (this->val == nullptr)
            {
                composite<T>* p = parent.on_add();
                this->val = new subdim<T, DefVal>(p, this->ind);
                p->add(this->val);
            }
            return this->val;
        }

        virtual void on_remove(composite<T>* v)
        {
            this->val->remove(v);
        }
};

template <typename T, T DefVal>
class index_proxy<T, DefVal, 1> : public index_proxy_base<T>
{
    private:
        index_proxy_base<T>& parent;

    public:
        index_proxy(index_proxy_base<T>& p, std::size_t index = -1, composite<T>* c = nullptr) : index_proxy_base<T>(c, index), parent(p)
        {
        }

        operator T(void) const
        {
            if (this->val != nullptr)
                return this->val->value();
            return DefVal;
        }

        index_proxy<T, DefVal, 1> operator=(const T& v)
        {
            if (v == DefVal)
            {
                if (this->val != nullptr)
                {
                    parent.on_remove(this->val);
                    this->val = nullptr;
                }
            }
            else if (this->val == nullptr)
            {
                this->val = new mvalue<T, DefVal>(v, this->ind);
                parent.on_add()->add(this->val);
            }
            else this->val->set_val(v);

            return *this;
        }

        bool operator==(const T& v) const
        {
            if (this->val == nullptr)
                return v == DefVal;
            return this->val->value() == v;
        }
};


template <typename T, T DefVal>
class index_proxy<T, DefVal, 0> : public index_proxy_base<T>
{
    public:
        index_proxy(composite<T>* val) : index_proxy_base<T>(val, 0)
        {
        }
};
