#include <iostream>
#include <stdexcept>

#include "dbops.h"
#include "dbexception.h"

#include "session.h"

namespace
{

operation* do_make_operation(const std::string& name, operation_args& args)
{
    std::unique_ptr<operation> op;
    if (name == "INSERT")
        op.reset(new op_insert);
    else if (name == "TRUNCATE")
        op.reset(new op_truncate);
    else if (name == "INTERSECTION")
    {
        op.reset(new op_intersect);
        args.push_back("A");
        args.push_back("B");
    }
    else if (name == "SYMMETRIC_DIFFERENCE")
    {
        args.push_back("A");
        args.push_back("B");
        op.reset(new op_sym_diff);
    }
    else if (name == "FREEZE")
    {
        op.reset(new op_freeze);
    }
    else 
        throw std::runtime_error("invalid operation");
    op->initialize(args);
    return op.release();
}

void do_execute(operation& op, database& db, lock_manager& lm, asio_socket& socket)
{
    table_name_list tablst = op.affected();
    table_list tabs;
    for (auto v : tablst)
    {
        table* t = &db.find(v);
        tabs.push_back(t);
    }
    operation_result res;
    lock_manager_guard g(tabs, lm, op.modification());
    op.apply(res, tabs);
    auto prn = [ &socket ](const result_record& rec)
    {
        std::stringstream ss;
        ss << rec.id << ",";
        if (rec.name_a != nullptr)
            ss << *rec.name_a;
        ss << ",";
        if (rec.name_b != nullptr)
            ss << *rec.name_b;
        ss << std::endl;
        boost::system::error_code error;
        boost::asio::write(socket, boost::asio::buffer(ss.str()), error);
    };
    std::for_each(res.begin(), res.end(), prn);
}

void do_process_command(const std::string& cmd, operation_args& args, database& db, lock_manager& lm, asio_socket& socket)
{
    std::unique_ptr<operation> op(do_make_operation(cmd, args));
    do_execute(*op.get(), db, lm, socket);
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer("OK\n", 3), error);
}

void do_parse(const std::string& str, std::string& name, operation_args& args)
{
    std::istringstream buf(str);
    std::istream_iterator<std::string> beg(buf);
    std::istream_iterator<std::string> end{};
    name = *beg++;
    args = std::move(operation_args(beg, end));
}

}

session::session(iosvc& io_service, database& db, lock_manager& lm) : m_socket(io_service), m_db(db), m_lm(lm)
{
}

session::~session() noexcept
{
}

asio_socket& session::get_socket()
{
    return m_socket;
}

void session::start()
{
    boost::asio::async_read_until(m_socket, m_buffer, '\n', boost::bind(&session::handle_read, this, ph_error, ph_trans));
}

void session::process_command(const std::string& cmd)
{
    try
    {
        std::string name;
        operation_args args;
        do_parse(cmd, name, args);
        do_process_command(name, args, m_db, m_lm, m_socket);
    }
    catch (const std::exception& ex)
    {
        std::stringstream ss;
        ss << "ERR " << ex.what() << std::endl;
        boost::system::error_code error;
        boost::asio::write(m_socket, boost::asio::buffer(ss.str()), error);
    }
    catch (...)
    {
        std::stringstream ss;
        ss << "ERR " << "unexpected error occured" << std::endl;
        boost::system::error_code error;
        boost::asio::write(m_socket, boost::asio::buffer(ss.str()), error);
    }
}

void session::handle_read(const error_code& error, std::size_t bytes_transferred)
{
    if (bytes_transferred > 0)
    {
        std::ostringstream ss;
        const char* part = boost::asio::buffer_cast<const char*>(m_buffer.data());
        ss.write(part, bytes_transferred);
        m_buffer.consume(bytes_transferred);
        process_command(ss.str());
    }

    if (error)
        delete this;
    else
        boost::asio::async_read_until(m_socket, m_buffer, '\n', boost::bind(&session::handle_read, this, ph_error, ph_trans));
}
