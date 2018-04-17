#pragma once

#include <stdexcept>

class dbexception : public std::exception
{
    public:
        virtual ~dbexception() noexcept { }
};

class duplicate_pk : public dbexception
{
    private:
        std::string m_text;

    public:
        duplicate_pk(const std::string& table, int id);
        const char* what() const throw() override;
};

class table_not_found : public dbexception
{
    private:
        std::string m_text;

    public:
        table_not_found(const std::string& tab_name);
        const char* what() const throw() override;
};

class record_not_found : public dbexception
{
    private:
        std::string m_text;

    public:
        record_not_found(const std::string& tab_name, int id);
        const char* what() const throw() override;    
};

class invalid_args_count : public dbexception
{
    private:
        std::string m_text;

    public:
        invalid_args_count(const std::string& tab_name, int count);
        const char* what() const throw() override;
};
